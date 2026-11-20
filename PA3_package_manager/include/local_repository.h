#ifndef LOCAL_REPOSITORY_H
#define LOCAL_REPOSITORY_H

#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "expected.h"
#include "package.h"
#include "repository.h"
#include "trie.h"

/**
 * @brief A locally installed package.
 */
struct InstalledPackage final {
  /**
   * @brief The installed package.
   */
  Package package;

  /**
   * @brief Whether the package was explicitly installed by the user, or installed as a dependency of another package.
   */
  bool explicit_install;
};

/**
 * @brief A local package repository, housing the set of packages installed on the system.
 */
class LocalRepository final : public Repository {
 public:
  /**
   * @brief Reads the local index from a file.
   *
   * Each line of the index consists of `<pkgname> <pkgver> [explicit]`, and is parsed into an `InstalledPackage`.
   *
   * All packages parsed from the index are first resolved against the remote repository by invoking `resolve` on the
   * name of the package. A resolved package is added into the local repository while keeping the version declared in
   * the index, while an unresolved package only retains the package name and version from the index.
   *
   * The index file may be absent, which is treated as an empty repository. The index file will be created on the first
   * `save`.
   *
   * @param index_path Path to the local index file; will be created on the first `save` if not present.
   * @param resolve Function that looks a package name up in the remote repositories, returning `nullptr` if none
   * provides it.
   * @return The loaded repository, or an error message describing the reason for failure.
   */
  static Expected<LocalRepository, std::string> load(const std::filesystem::path& index_path,
                                                     const std::function<const Package*(const std::string&)>& resolve);

  [[nodiscard]] const std::filesystem::path& get_path() const override { return path; }
  [[nodiscard]] std::optional<const Package*> search(const std::string& name) const override;
  [[nodiscard]] std::vector<const Package*> list() const override;

  /**
   * @brief Hashes `text` with FNV-1a 64.
   *
   * Note that each character should be hashed as an unsigned byte (`unsigned char`).
   *
   * @param text The text to hash.
   * @return The 64-bit hash.
   */
  [[nodiscard]] static unsigned long long fnv1a(const std::string& text);

  /**
   * @param name The package name.
   * @return True if the package was installed explicitly as per the user's request, false when it is installed as a
   * dependency, and `std::nullopt` if it is not installed.
   */
  [[nodiscard]] std::optional<bool> is_explicit(const std::string& name) const {
    PackageTrie::const_iterator found{packages.find(name)};
    if (found == packages.end()) {
      return std::nullopt;
    }
    return found->explicit_install;
  }

  /**
   * @brief Writes the current index into a file on the filesystem.
   *
   * The write is performed via a temporary file and renamed, so a failed write leaves the previous index intact.
   *
   * @param index_path The local index file to write.
   * @return `std::nullopt` on success, or an error message describing the failure.
   */
  [[nodiscard]] std::optional<std::string> save(const std::filesystem::path& index_path) const;

  /**
   * @brief Lists the installed packages that depend on `name`.
   *
   * @param name The dependency name.
   * @return The package names of the dependents.
   */
  [[nodiscard]] std::vector<std::string> dependents(const std::string& name) const {
    std::map<std::string, std::vector<std::string>>::const_iterator found{reverse_depends.find(name)};
    if (found == reverse_depends.end()) {
      return {};
    }
    return found->second;
  }

  /**
   * @brief Stores `package` into `packages`, replacing any installed package of the same name, and updates the
   * reverse-dependency map.
   *
   * Replacing a package should only unlink the old version's own dependencies; entries recording the collection of
   * other packages that depend on this package are kept as-is.
   *
   * @param package The package to store.
   * @param explicit_install Whether the user explicitly requested this package to be installed.
   */
  void put(const Package& package, bool explicit_install);

  /**
   * @brief Removes a package without any dependency checks.
   *
   * @param name The name of the package.
   */
  void drop(const std::string& name);

 private:
  /**
   * @brief The data structure used to store installed packages.
   *
   * The trie consists of 17 buckets: Buckets 1..16 for the hash and name nibbles and 0 reserved for end-of-name.
   */
  using PackageTrie = Trie<InstalledPackage, std::string, 17>;

  /**
   * @brief Computes the bucket index for a package name at a given depth in the trie.
   *
   * The hash used in the trie is a variable-length hash: The first 16 nibbles are derived from the FNV-1a hash,
   * followed by the ASCII bytes of the name converted into nibbles, and terminated by the end-of-name marker.
   *
   * @param name The package name.
   * @param depth The depth in the trie.
   * @return The bucket index, in `[0, 17)`.
   */
  static std::size_t package_bucket(const std::string& name, std::size_t depth);

  /**
   * @brief Builds an empty repository bound to an index path.
   *
   * @param path The index path.
   */
  explicit LocalRepository(std::filesystem::path path);

  /**
   * @brief Removes `dependent` from a dependency's reverse-dependency list.
   *
   * Note that an empty `reverse_depends` entry should be erased from the map (rather than leaving it empty).
   *
   * @param dependency The dependency name.
   * @param dependent The dependent name to remove.
   */
  void unlink(const std::string& dependency, const std::string& dependent);

  std::filesystem::path path;
  PackageTrie packages;
  std::map<std::string, std::vector<std::string>> reverse_depends;
};

#endif  // LOCAL_REPOSITORY_H
