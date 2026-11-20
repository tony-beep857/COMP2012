#include "version.h"

bool PkgVersion::operator==(const PkgVersion& other) const {
  // TODO
  if (major == other.major && minor == other.minor && patch == other.patch)
    return true;
  return false;
}

bool PkgVersion::operator<(const PkgVersion& other) const {
  // TODO
  if (major < other.major)
    return true;
  if (major == other.major && minor < other.minor)
    return true;
  if (major == other.major && minor == other.minor && patch < other.patch)
    return true;
  return false;
}

PkgVersion::operator std::string() const {
  // TODO
  return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
}

std::ostream& operator<<(std::ostream& os, const PkgVersion& version) {
  // TODO
  os << std::string{version};
  return os;
}
