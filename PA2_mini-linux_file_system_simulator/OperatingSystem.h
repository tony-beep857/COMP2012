#ifndef __OPERATING_SYSTEM_H__
#define __OPERATING_SYSTEM_H__

#include <vector>

#include "FileSystem.h"
#include "Group.h"
#include "User.h"

enum ExecResult {
  Continue,
  Logout,
  Shutdown,
};

class OperatingSystem {
  private:
    std::vector<User*> users;
    std::vector<Group*> groups;

    FileSystem* filesys;
    User* cur_user;

    void groupadd(const std::string& group_name);

    User* su(const std::string& target_name);

    void adduser(const std::string& user);

    void usermod(const std::string& user, const std::vector<std::string>& groups, bool append);

    User* search_user(const std::string& name) const;
    Group* search_group(const std::string& name) const;
  public:
    OperatingSystem();

    ~OperatingSystem();

    OperatingSystem(const OperatingSystem&) = delete;

    ExecResult execute(const std::string& input);

    bool login(const std::string& name);

    void show_shell_prompt() const;
};

#endif // __OPERATING_SYSTEM_H__
