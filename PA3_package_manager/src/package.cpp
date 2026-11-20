#include "package.h"

#include <optional>

#include "util.h"

using std::operator""s;

namespace {
/**
 * @brief Splits a space-separated name list into a set, dropping empty fields.
 *
 * @param value The space-separated names.
 * @return The names, without duplicates.
 */
std::set<std::string> split_names(std::string_view value) {
  std::set<std::string> names;
  for (const std::string_view name : split(value, ' ')) {
    if (!name.empty()) {
      names.emplace(name);
    }
  }
  return names;
}
}  // namespace

Expected<Package, std::string> Package::parse(std::string_view text) {
  std::string name;
  std::string desc;
  std::optional<PkgVersion> version;
  std::set<std::string> depends;
  std::set<std::string> conflicts;
  bool has_name{false};
  bool has_desc{false};

  for (std::string_view line : split(text, '\n')) {
    if (!line.empty() && line.back() == '\r') {
      line.remove_suffix(1);
    }
    if (line.empty()) {
      continue;
    }

    const std::size_t equals{line.find('=')};
    if (equals == std::string_view::npos) {
      return Unexpected{"Missing '=' in package line: "s + std::string{line}};
    }

    const std::string_view key{line.substr(0, equals)};
    const std::string_view value{line.substr(equals + 1)};

    if (key == "pkgname") {
      name = value;
      has_name = true;
    } else if (key == "desc") {
      desc = value;
      has_desc = true;
    } else if (key == "pkgver") {
      Expected<PkgVersion, std::string> parsed{PkgVersion::parse(std::string{value})};
      if (!parsed) {
        return Unexpected{parsed.error()};
      }
      version = parsed.value();
    } else if (key == "depends") {
      depends = split_names(value);
    } else if (key == "conflicts") {
      conflicts = split_names(value);
    } else {
      return Unexpected{"Unknown package key: "s + std::string{key}};
    }
  }

  if (!has_name) {
    return Unexpected{"Missing pkgname"s};
  }
  if (!has_desc) {
    return Unexpected{"Missing desc"s};
  }
  if (!version) {
    return Unexpected{"Missing pkgver"s};
  }

  return Package{std::move(name), std::move(desc), *version, std::move(depends), std::move(conflicts)};
}
