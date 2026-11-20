#ifndef VERSION_H
#define VERSION_H

#include <string>

#include "expected.h"

/**
 * @brief A `major.minor.patch` version number.
 */
class PkgVersion final {
 public:
  /**
   * @brief Whether every component is equal.
   *
   * @param other The version to compare against.
   * @return True when the two versions are identical.
   */
  bool operator==(const PkgVersion& other) const;
  bool operator!=(const PkgVersion& other) const { return !(*this == other); };

  /**
   * @brief Orders by major, then minor, then patch.
   *
   * @param other The version to compare against.
   * @return True when this version is older.
   */
  bool operator<(const PkgVersion& other) const;
  bool operator<=(const PkgVersion& other) const { return *this < other || *this == other; };
  bool operator>(const PkgVersion& other) const { return !(*this <= other); };
  bool operator>=(const PkgVersion& other) const { return !(*this < other); };

  /**
   * @return The major version number.
   */
  [[nodiscard]] unsigned long get_major() const { return major; }

  /**
   * @return The minor version number.
   */
  [[nodiscard]] unsigned long get_minor() const { return minor; }

  /**
   * @return The patch version number.
   */
  [[nodiscard]] unsigned long get_patch() const { return patch; }

  /**
   * @brief Formats the version as `major.minor.patch`.
   *
   * @return The dotted form.
   */
  explicit operator std::string() const;

  /**
   * @brief Parses a `major.minor.patch` string.
   *
   * Every component must be a non-empty run of decimal digits, separated by single dots, with nothing after the patch
   * component.
   *
   * @param ver The text to parse.
   * @return The parsed version, or a message naming the failing component.
   */
  static Expected<PkgVersion, std::string> parse(const std::string& ver);

 private:
  /**
   * @brief Builds a version from its components.
   *
   * @param major The major version number.
   * @param minor The minor version number.
   * @param patch The patch version number.
   */
  explicit PkgVersion(unsigned long major, unsigned long minor, unsigned long patch);

  /**
   * @brief Writes the dotted form.
   *
   * @param os The stream to write to.
   * @param version The version to write.
   * @return The stream, for chaining.
   */
  friend std::ostream& operator<<(std::ostream& os, const PkgVersion& version);

  unsigned long major;
  unsigned long minor;
  unsigned long patch;
};

#endif  // VERSION_H
