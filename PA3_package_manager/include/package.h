#ifndef PACKAGE_H
#define PACKAGE_H

#include <set>
#include <string>
#include <string_view>
#include <utility>

#include "expected.h"
#include "version.h"

/**
 * @brief A package record.
 */
class Package final {
 public:
  /**
   * @brief Builds a package record.
   *
   * @param name The package name.
   * @param desc A human-readable description.
   * @param version The package version.
   * @param depends Packages that this package requires.
   * @param conflicts Packages that this package must not be installed alongside.
   */
  Package(std::string name,
          std::string desc,
          PkgVersion version,
          std::set<std::string> depends = {},
          std::set<std::string> conflicts = {}) :
      name{std::move(name)},
      desc{std::move(desc)},
      version{version},
      depends{std::move(depends)},
      conflicts{std::move(conflicts)} {}

  /**
   * @return The name of this package.
   */
  [[nodiscard]] const std::string& get_name() const { return name; }

  /**
   * @return The description of this package.
   */
  [[nodiscard]] const std::string& get_desc() const { return desc; }

  /**
   * @return The version of this package.
   */
  [[nodiscard]] const PkgVersion& get_version() const { return version; }

  /**
   * @return The packages that this package requires.
   */
  [[nodiscard]] const std::set<std::string>& get_depends() const { return depends; }

  /**
   * @return The names this package cannot be be installed alongside.
   */
  [[nodiscard]] const std::set<std::string>& get_conflicts() const { return conflicts; }

  /**
   * @brief Parses a package file.
   *
   * Blank lines and a trailing carriage return are ignored; a repeated key keeps the last value. Package names
   * in `depends` and `conflicts` are space-delimited.
   *
   * @param text The contents of a package file.
   * @return The parsed package, or an error message describing the issue.
   */
  static Expected<Package, std::string> parse(std::string_view text);

 private:
  std::string name;
  std::string desc;
  PkgVersion version;
  std::set<std::string> depends;
  std::set<std::string> conflicts;
};

#endif  // PACKAGE_H
