#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

#include "expected.h"
#include "local_repository.h"
#include "package.h"
#include "remote_repository.h"

/**
 * @brief The outcome of an operation that can be refused for several distinct reasons.
 */
enum class Status {
  /**
   * @brief The operation completed.
   */
  SUCCESS,

  /**
   * @brief A conflict blocked it.
   */
  CONFLICT,

  /**
   * @brief Another installed package still depends on the target.
   */
  BREAKS_DEPS,

  /**
   * @brief The package, or one of its dependencies, is unavailable.
   */
  NOT_FOUND,

  /**
   * @brief The package is installed already.
   */
  ALREADY_INSTALLED,
};

/**
 * @brief The abstraction over a package manager.
 */
class PackageManager final {
 public:
  ~PackageManager() = default;

  PackageManager(const PackageManager&) = delete;
  PackageManager& operator=(const PackageManager&) = delete;
  PackageManager(PackageManager&&) noexcept = default;
  PackageManager& operator=(PackageManager&&) noexcept = default;

  /**
   * @brief Creates an instance of the package manager by opening the local and remote indexes.
   *
   * @param local_index The local index file; may be absent.
   * @param remote_index The remote index file.
   * @return The created instance, or an error message describing the failure.
   */
  static Expected<PackageManager, std::string> create(const std::filesystem::path& local_index,
                                                      const std::filesystem::path& remote_index);

  /**
   * @brief Installs a package with the given name, alongside its missing dependencies as dependencies (i.e. not
   * explicitly installed).
   *
   * The to-be-installed packages (or their dependencies) must not conflict with an installed package or with one
   * another. The local repository should only be modified once conflict detection has been performed and no conflicts
   * have been found.
   *
   * @param name The name of the package to install.
   * @param explicit_install Whether the user asked for it, rather than it being pulled in as a dependency.
   * @return `SUCCESS` if the package is successfully installed, `ALREADY_INSTALLED` if the package is already installed
   * (regardless of version), `NOT_FOUND` if the package (or any dependency) is not found in the remote repository, or
   * `CONFLICT` if the package (or any dependency) conflicts with an installed package or with another incoming package.
   */
  Status install(const std::string& name, bool explicit_install);

  /**
   * @brief Removes the package with the given name, then recursively removes dependencies that were installed as
   * dependencies and that nothing requires any more.
   *
   * Note that explicitly installed dependencies are never removed.
   *
   * @param name The package to remove.
   * @return `SUCCESS` if the package is successfully removed, `NOT_FOUND` if the requested package is not installed, or
   * `BREAKS_DEPS` if the package is still required by another installed package.
   */
  Status uninstall(const std::string& name);

  /**
   * @brief Updates all installed packages by replacing them with the latest versions from the remote repository,
   * keeping the installation flag, and installs the new versions' missing dependencies (as not explicitly installed) if
   * needed.
   *
   * Note that the local repository should only be modified if no upgraded package or new dependency conflicts with the
   * upgraded package set. Moreover, an upgrade never removes packages.
   *
   * @return `SUCCESS` if the upgrade is successful, `NOT_FOUND` if a dependency of an upgraded package is not found in
   * the remote repository, or `CONFLICT` if an upgraded package or one of its new dependencies conflicts.
   */
  Status upgrade();

  /**
   * @brief Removes all packages installed as dependencies that no package requires, repeating until no further package
   * becomes unused.
   *
   * @return The number of packages removed.
   */
  std::size_t prune();

  /**
   * @brief Lists the installed packages that the remote repository does not provide.
   *
   * @return The sorted names of orphaned packages.
   */
  [[nodiscard]] std::vector<std::string> orphans() const;

  /**
   * @return The local repository.
   */
  [[nodiscard]] const LocalRepository& get_local() const { return local; }

  /**
   * @return The remote repository.
   */
  [[nodiscard]] const RemoteRepository& get_remote() const { return remote; }

 private:
  /**
   * @brief Builds a manager from a loaded local repository and remote.
   *
   * @param local The installed set.
   * @param remote The remote repository.
   */
  PackageManager(LocalRepository local, RemoteRepository remote) : local{std::move(local)}, remote{std::move(remote)} {}

  /**
   * @brief Collects all dependencies of `package` that are not currently installed in a local repository.
   *
   * @param package The package whose dependencies to resolve.
   * @param repository The local repository containing the set of installed packages.
   * @return The missing packages, or `std::nullopt` when the remote repository does not provide a dependency.
   */
  [[nodiscard]] std::optional<std::vector<Package>> collect_missing(const Package& package,
                                                                    const LocalRepository& repository) const;

  /**
   * @brief Checks whether `package` conflicts with an installed package in either direction: an installed package
   * conflicts with it, or it conflicts with an installed one.
   *
   * @param package The candidate package.
   * @param repository The local repository containing the set of already-installed packages.
   * @return True when a conflict exists in either direction.
   */
  [[nodiscard]] static bool conflicts(const Package& package, const LocalRepository& repository);

  /**
   * @brief Removes the package with the given name from the local repository, then removes every dependency that was
   * installed as a dependency and that nothing requires any more.
   *
   * @param name The package to remove; removing a not-installed package does nothing.
   */
  void remove_with_unused_dependencies(const std::string& name);

  LocalRepository local;
  RemoteRepository remote;
};

#endif  // PACKAGE_MANAGER_H
