#ifndef __DIAGNOSE_H__
#define __DIAGNOSE_H__

#include <string>

namespace diagnose {
  enum ErrorType {
    PermissionDenied,
    NoFileOrDirectory,
    FileOrDirectoryExists,
    NoAtFile,
    NotADirectory,
    NoRecursionSpecified,
    NoUserOrGroup,
    UserOrGroupExists
  };

  void report(const std::string& cmd, const std::string& target, ErrorType type);
} // namespace diagnose

#endif // __DIAGNOSE_H__
