#include "OperatingSystem.h"

#include <iostream>
#include <vector>

#include "Diagnose.h"

using diagnose::ErrorType;
using diagnose::report;

const std::string white_chars{" \t\r\n"};
inline static std::string strip(std::string s) {
  // Strip right
  std::string::size_type right{s.find_last_not_of(white_chars)};
  s = right != std::string::npos ? s.substr(0, right + 1) : "";
  // Strip left
  std::string::size_type left{s.find_first_not_of(white_chars)};
  s = left != std::string::npos ? s.substr(left) : "";
  return s;
}

bool OperatingSystem::login(const std::string& name) {
  User *u = search_user(name);
  if (u == nullptr) {
    std::cout << "User does not exist." << std::endl;
    return false;
  }
  int tries = 0;
  std::string buf;
  std::cout << "Logging in as " << name << "@pa2.comp2012.ust.hk...\n";
  while (tries < 3) {
      std::cout << "Please enter the password: ";
      std::getline(std::cin, buf);
      if (u->match_password(buf)) {
        cur_user = u;
        return true;
      }
      std::cout << "Incorrect password. ";
      ++tries;
  }
  std::cout << "\nTries exceeded. Exiting." << std::endl;
  return false;
}

ExecResult OperatingSystem::execute(const std::string& input) {
  std::string buf, stripped = strip(input);
  std::vector<std::string> command;
  unsigned int i = 0, j = 0;
  for (; j < stripped.size(); ++j) {
    if (stripped[j] == ' ') {
      buf = stripped.substr(i, j - i);
      if (!buf.empty()) {
        command.emplace_back(buf);
      }

      i = j + 1;
    }
  }
  command.emplace_back(stripped.substr(i));

  if (command[0].empty()) {
    return ExecResult::Continue;
  }
  if (command[0] == "exit") {
    if (command.size() == 1) { return ExecResult::Logout; }
    std::cout << "exit: invalid arguments" << std::endl;
    return ExecResult::Continue;
  }
  if (command[0] == "shutdown") {
    if (command.size() == 1) { return ExecResult::Shutdown; }
    std::cout << "shutdown: invalid arguments" << std::endl;
    return ExecResult::Continue;
  }

  if (command[0] == "cat") {
    /**
     * cat FILE
     */
    if (command.size() == 2) {
      filesys->cat(*cur_user, command[1]);
      return ExecResult::Continue;
    }
    else {
      std::cout << "cat: invalid arguments" << std::endl;
      return ExecResult::Continue;
    }
  }
  else if (command[0] == "cd") {
    /**
     * cd DIRECTORY
     */
    if (command.size() == 2) {
      filesys->cd(*cur_user, command[1]);
    }
    else {
      std::cout << "cd: invalid arguments" << std::endl;
    }
  }
  else if (command[0] == "chmod") {
    /**
     * chmod [-R] PERMISSION FILE
     */
    if (command.size() >= 3) {
      unsigned int start_idx = 1;
      bool recursive = false;
      if (command[1][0] == '-') {
        if (command[1][1] == 'R') {
          recursive = true;
          ++start_idx;
        }
        else {
          std::cout << "chmod: invalid option -- '" << command[1].substr(1) << "'" << std::endl;
          return ExecResult::Continue;
        }
      }

      if (start_idx + 1 < command.size()) {
        filesys->chmod(*cur_user, command[start_idx + 1], command[start_idx], recursive);
        return ExecResult::Continue;
      }
    }

    std::cout << "chmod: invalid arguments" << std::endl;
  }
  else if (command[0] == "cp") {
    /**
     * cp [-p] [-r|R] SRC DEST
     */
    bool recursive{false}, preserve_perms{false};
    unsigned int start_idx{1};
    for (; start_idx < command.size() && command[start_idx][0] == '-'; ++start_idx) {
      if (command[start_idx].find('r') != std::string::npos || command[start_idx].find('R') != std::string::npos)
          recursive = true;
      if (command[start_idx].find('p') != std::string::npos)
          preserve_perms = true;
    }
    if (start_idx + 1 < command.size()) {
      filesys->cp(*cur_user, command[start_idx], command[start_idx + 1], recursive, preserve_perms);
    } else if (start_idx + 1 == command.size()) {
      std::cout << "cp: missing destination file operand after '" << command[start_idx] << "'" << std::endl;
    } else {
      std::cout << "cp: missing operand" << std::endl;
    }
  }
  else if (command[0] == "groupadd") {
    /**
     * groupadd GROUP
     */
    if (command.size() == 2) {
      groupadd(command[1]);
    }
    else {
      std::cout << "groupadd: invalid arguments" << std::endl;
    }
  }
  else if (command[0] == "ls") {
    /**
     * ls [-a] [-l] [-R] [FILE...]
     */
    bool all{false}, long_format{false}, recursive{false};
    path_t targets;
    for (unsigned int start_idx{1}; start_idx < command.size(); ++start_idx) {
      if (command[start_idx][0] == '-') {
        if (command[start_idx].find('a') != std::string::npos) all = true;
        if (command[start_idx].find('l') != std::string::npos) long_format = true;
        if (command[start_idx].find('R') != std::string::npos) recursive = true;
      }
      else {
        targets.push_back(command[start_idx]);
      }
    }
    filesys->ls(*cur_user, targets, all, long_format, recursive);
  }
  else if (command[0] == "mkdir") {
    /**
     * mkdir [-p] DIRECTORY...
     */
    bool create_parents{false};
    path_t targets;
    unsigned int start_idx{1};
    for (; start_idx < command.size() && command[start_idx][0] == '-'; ++start_idx) {
      if (command[start_idx].find('p') != std::string::npos) create_parents = true;
    }
    for (; start_idx < command.size(); ++start_idx) {
      if (command[start_idx][0] == '-') {
          std::cout << "mkdir: invalid option -- '" << command[start_idx].substr(1) << "'" << std::endl;
          return ExecResult::Continue;
      }
      targets.emplace_back(command[start_idx]);
    }
    if (targets.size() > 0) {
      filesys->mkdir(*cur_user, targets, create_parents);
    }
    else {
      std::cout << "mkdir: missing operand" << std::endl;
    }
  }
  else if (command[0] == "touch") {
    /**
     * touch FILE...
     */
    if (command.size() >= 2) {
      path_t targets;
      for (unsigned int start_idx{1}; start_idx < command.size(); ++start_idx) {
        if (command[start_idx][0] == '-') {
          std::cout << "touch: invalid option -- '" << command[start_idx].substr(1) << "'" << std::endl;
          return ExecResult::Continue;
        }
        targets.emplace_back(command[start_idx]);
      }
      filesys->touch(*cur_user, targets);
    }
    else {
      std::cout << "touch: missing file operand" << std::endl;
    }
  }
  else if (command[0] == "rm") {
    /**
     * rm [-r|R] FILE
     */
    bool recursive{false};
    path_t targets;
    for (unsigned int start_idx{1}; start_idx < command.size(); ++start_idx) {
      if (command[start_idx][0] == '-') {
        if (command[start_idx].find('r') != std::string::npos || command[start_idx].find('R') != std::string::npos) {
          recursive = true;
        }
        else {
          std::cout << "rm: invalid option -- '" << command[start_idx].substr(1) << "'" << std::endl;
          return ExecResult::Continue;
        }
      }
      else {
        targets.emplace_back(command[start_idx]);
      }
    }
    if (!targets.empty())
      filesys->rm(*cur_user, targets, recursive);
    else
      std::cout << "rm: missing operand" << std::endl;
  }
  else if (command[0] == "su") {
    /**
     * su USER
     */
    if (command.size() == 2) {
      User *new_user{su(command[1])};
      if (new_user)
        cur_user = new_user;
    }
    else {
      std::cout << "su: invalid arguments" << std::endl;
    }
  }
  else if (command[0] == "adduser") {
    /**
     * adduser LOGIN
     */
    if (command.size() == 2) {
      adduser(command[1]);
    } else {
      std::cout << "adduser: invalid arguments" << std::endl;
    }
  }
  else if (command[0] == "usermod") {
    /**
     * usermod [-a] -G GROUP1[,GROUP2,...[,GROUPN]]]
     */
    bool append{false};
    std::string groups_str;
    std::string login;
    unsigned int k{1};
    while (k < command.size()) {
      if (command[k][0] == '-' && command[k].size() > 1) {
        bool need_groups{false};
        for (std::string::size_type c{1}; c < command[k].size(); ++c) {
          if (command[k][c] == 'a')
            append = true;
          else if (command[k][c] == 'G')
            need_groups = true;
          else {
            std::cout << "usermod: invalid option -- '" << command[k][c] << "'" << std::endl;
            return ExecResult::Continue;
          }
        }
        ++k;
        if (need_groups && k < command.size())
          groups_str = command[k++];
      }
      else if (command[k][0] != '-') {
        login = command[k++];
      }
      else {
        std::cout << "usermod: invalid arguments" << std::endl;
        return ExecResult::Continue;
      }
    }

    if (groups_str.empty() || login.empty()) {
      std::cout << "usermod: invalid arguments" << std::endl;
      return ExecResult::Continue;
    }

    std::vector<std::string> new_groups;
    std::string::size_type start{0}, end;
    while ((end = groups_str.find(',', start)) != std::string::npos) {
      std::string name = groups_str.substr(start, end - start);
      new_groups.emplace_back(name);
      start = end + 1;
    }

    std::string name = groups_str.substr(start);
    new_groups.emplace_back(name);

    usermod(login, new_groups, append);
  }
  else
  {
    std::cout << command[0] << ": command not found" << std::endl;
  }
  return ExecResult::Continue;
}

void OperatingSystem::show_shell_prompt() const {
  std::cout << cur_user->get_name() << ":" << filesys->pwd() << "$ ";
}

User* OperatingSystem::su(const std::string& target_name) {
  User* target = search_user(target_name);
  if (target == nullptr) {
    report(__func__, target_name, ErrorType::NoUserOrGroup);
    return nullptr;
  }

  std::string buffer;
  int tries = 0;
  while (tries < 3) {
    std::cout << "Password: ";
    std::getline(std::cin, buffer);
    if (target->match_password(buffer)) {
      return target;
    }
    std::cout << "su: Authentication failure" << std::endl;
    ++tries;
  }
  std::cout << "su: Maximum attempts exceeded" << std::endl;
  return nullptr;
}

void OperatingSystem::adduser(const std::string& user) {
  if (!cur_user->is_root()) {
    report(__func__, user, ErrorType::PermissionDenied);
    return;
  }

  if (search_user(user) != nullptr) {
    report(__func__, user, ErrorType::UserOrGroupExists);
    return;
  }

  std::string password;
  std::cout << "New password: ";
  std::getline(std::cin, password);

  Group* g = groups.emplace_back(new Group{user});
  User* u = users.emplace_back(new User{user, password, *g});
  g->add_user(*u);

  if (user != "root") {
    filesys->add_user_dir(*cur_user, *u, __func__);
  }
}
