#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <filesystem>
#include <optional>
#include <vector>

#include "package.h"

/**
 * @brief A set of packages that can be searched and listed.
 *
 * Subclasses own the packages and define the storage and lookup strategy. The packages stay owned by the repository
 * object: the pointers `search` and `list` return are borrowed and live only as long as the repository does.
 */
class Repository {
 public:
  Repository() = default;
  virtual ~Repository() = 0;

  Repository(const Repository&) = default;
  Repository(Repository&&) noexcept = default;

  Repository& operator=(const Repository&) = default;
  Repository& operator=(Repository&&) noexcept = default;

  /**
   * @return The path to the repository's index file.
   */
  [[nodiscard]] virtual const std::filesystem::path& get_path() const = 0;

  /**
   * @brief Looks a package up by name.
   *
   * @param name The package name.
   * @return The package, or `std::nullopt` when the repository does not contain it.
   */
  [[nodiscard]] virtual std::optional<const Package*> search(const std::string& name) const = 0;

  /**
   * @return The list of packages in the repository, in the repository's order.
   */
  [[nodiscard]] virtual std::vector<const Package*> list() const = 0;
};

inline Repository::~Repository() = default;

#endif  // REPOSITORY_H
