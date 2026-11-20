#ifndef REMOTE_REPOSITORY_H
#define REMOTE_REPOSITORY_H

#include <filesystem>
#include <optional>
#include <utility>
#include <vector>

#include "expected.h"
#include "package.h"
#include "repository.h"

/**
 * @brief A remote repository, housing the set of available packages from a remote source (e.g. over the Internet).
 */
class RemoteRepository final : public Repository {
 public:
  /**
   * @brief Opens a remote repository from its index file.
   *
   * Every name in the index is read from the package file next to it; blank lines and trailing carriage returns are
   * ignored.
   *
   * @param index_path The path to the index file, whose directory holds the package files.
   * @return The created repository instance, or an error message describing the failure.
   */
  static Expected<RemoteRepository, std::string> open(const std::filesystem::path& index_path);

  [[nodiscard]] const std::filesystem::path& get_path() const override { return path; }
  [[nodiscard]] std::optional<const Package*> search(const std::string& name) const override;
  [[nodiscard]] std::vector<const Package*> list() const override;

 private:
  /**
   * @brief Builds a repository from an already-read index.
   *
   * @param path The index path.
   * @param packages The packages read from it.
   */
  RemoteRepository(std::filesystem::path path, std::vector<Package> packages) :
      path{std::move(path)}, packages{std::move(packages)} {}

  std::filesystem::path path;
  std::vector<Package> packages;
};

#endif  // REMOTE_REPOSITORY_H
