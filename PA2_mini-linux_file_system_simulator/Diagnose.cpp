#include "Diagnose.h"

#include <iostream>
#include <array>

namespace diagnose {
  static std::array<std::string, 8> error_msgs {
    "Permission denied",
    "No such file or directory",
    "File or directory exists",
    "Not a file",
    "Not a directory",
    "-r not specified",
    "No such user or group",
    "User or group exists"
  };

  void report(const std::string& cmd, const std::string& target, ErrorType type) {
    std::cout << cmd << ": '" << (target.empty() ? "/" : target) << "': " << error_msgs[static_cast<int>(type)] << std::endl;
  }
} // namespace diagnose
