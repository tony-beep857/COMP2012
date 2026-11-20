#include "version.h"

#include <functional>

#include "expected.h"

using std::operator""s;
using std::operator""sv;

PkgVersion::PkgVersion(unsigned long major, unsigned long minor, unsigned long patch) :
    major{major}, minor{minor}, patch{patch} {}

Expected<PkgVersion, std::string> PkgVersion::parse(const std::string& ver) {
  std::string::const_iterator iter{ver.cbegin()};

  const std::function<Expected<unsigned long, std::string>(std::string::const_iterator&, std::string_view)> parse_num{
      [&ver](std::string::const_iterator& iter,
             const std::string_view parse_part) -> Expected<unsigned long, std::string> {
        if (iter == ver.cend() || std::isdigit(*iter) == 0) {
          return Unexpected{"Unexpected empty number while parsing "s + std::string{parse_part}};
        }

        unsigned long value{0};
        while (iter != ver.cend() && std::isdigit(*iter) != 0) {
          value = (value * 10UL) + static_cast<unsigned long>(*iter - '0');
          ++iter;
        }
        return value;
      }};

  Expected<unsigned long, std::string> major_res{parse_num(iter, "major"sv)};
  if (!major_res) {
    return Unexpected{major_res.error()};
  }
  unsigned long major{*major_res};

  if (iter == ver.cend() || *iter != '.') {
    return Unexpected{"Unexpected character while parsing '.'"s};
  }
  ++iter;

  Expected<unsigned long, std::string> minor_res{parse_num(iter, "minor"sv)};
  if (!minor_res) {
    return Unexpected{minor_res.error()};
  }
  unsigned long minor{*minor_res};

  if (iter == ver.cend() || *iter != '.') {
    return Unexpected{"Unexpected character while parsing '.'"s};
  }
  ++iter;

  Expected<unsigned long, std::string> patch_res{parse_num(iter, "patch"sv)};
  if (!patch_res) {
    return Unexpected{patch_res.error()};
  }
  unsigned long patch{*patch_res};

  if (iter == ver.cend()) {
    return PkgVersion{major, minor, patch};
  }

  return Unexpected{"Unexpected character beyond end-of-string: "s + std::string{*iter}};
}
