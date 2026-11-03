#include "FileEntry.h"

#include <iostream>

#include "Diagnose.h"

using diagnose::report;
using diagnose::ErrorType;

std::optional<unsigned short>  FileEntry::parse_permission(const std::string &perm) const {
  unsigned short perm_num = 0;
  bool flag = true;
  if (perm.empty()) {
    flag = false;
  }
  else {
    bool is_octal = true;
    for (char c : perm) {
      if (c < '0' || c > '7') {
        is_octal = false;
        break;
      }
    }

    if (is_octal) {
      unsigned short p{0};
      for (char c : perm) {
          p = static_cast<unsigned short>((p << 3) | (c - '0'));
      }
      perm_num = p;
    }
    else if (perm.size() != 3) {
      flag = false;
    }
    else {
      unsigned short role_mask = 0, shift = 0;
      switch (perm[0]) {
        case 'u': role_mask = 0700; shift = 6; break;
        case 'g': role_mask = 0070; shift = 3; break;
        case 'o': role_mask = 0007; shift = 0; break;
        case 'a': role_mask = 0777; break;
        default: flag = false;
      }

      char op{perm[1]};
      if (op != '+' && op != '-' && op != '=') { flag = false; }

      unsigned short perm_pattern{0};
      switch (perm[2]) {
        case 'r': perm_pattern = 4; break;
        case 'w': perm_pattern = 2; break;
        case 'x': perm_pattern = 1; break;
        default: flag = false;
      }

      unsigned short perm_bits = (perm[0] == 'a') ? static_cast<unsigned short>((perm_pattern << 6) | (perm_pattern << 3) | perm_pattern)
        : static_cast<unsigned short>(perm_pattern << shift);

      switch (op) {
        case '+': perm_num = static_cast<unsigned short>(permissions | (perm_bits & role_mask)); break;
        case '-': perm_num = static_cast<unsigned short>(permissions & ~(perm_bits & role_mask)); break;
        case '=': perm_num = static_cast<unsigned short>((permissions & ~role_mask) | (perm_bits & role_mask)); break;
        default: ;
      }
    }
  }

  if (!flag) {
    perm_num = 0;
    std::cout << "chmod: invalid permissions: " << perm << std::endl;
    return std::nullopt;
  }

  return perm_num;
}


bool FileEntry::ls([[maybe_unused]] const User& user, [[maybe_unused]] bool all, [[maybe_unused]] bool recursive, const std::optional<FormatData>& long_format, const std::string& display_name) const {
  std::string name = display_name.empty() ? this->name : display_name;
  if (long_format.has_value()) {
    std::string owner = get_owner()->get_name();
    std::string group = get_group()->get_name();
    std::string size_str = std::to_string(get_size());

    std::string permission;
    for (int shift : {6, 3, 0}) {
      int bits = (permissions >> shift) & 7;
      permission += (bits & static_cast<unsigned short>(Permission::Read)) ? 'r' : '-';
      permission += (bits & static_cast<unsigned short>(Permission::Write)) ? 'w' : '-';
      permission += (bits & static_cast<unsigned short>(Permission::Exe)) ? 'x' : '-';
    }

    std::cout << get_type() << permission << " "
              << owner << std::string(long_format->max_owner - owner.size(), ' ') << " "
              << group << std::string(long_format->max_group - group.size(), ' ') << " "
              << std::string(long_format->max_size - size_str.size(), ' ') << size_str << " "
              << name << std::endl;
  }
  else {
    std::cout << name << std::endl;
  }

  return true;
}

bool FileEntry::has_permissions(const User& user, Permission perm, const std::string& cmd) const {
  bool flag = false;
  if (user.is_root()) {
    flag = true;
  }
  else if (owner->get_name() == user.get_name()) {
    flag = (permissions >> 6) & static_cast<unsigned short>(perm);
  }
  else if (group->has_user(user)) {
    flag = (permissions >> 3) % 8 & static_cast<unsigned short>(perm);
  }
  else {
    flag = (permissions % 8) & static_cast<unsigned short>(perm);
  }

  if (!flag) {
    report(cmd, get_name(), ErrorType::PermissionDenied);
  }

  return flag;
}
