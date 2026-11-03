#ifndef __USER_H__
#define __USER_H__

#include <string>
#include <vector>

class Group;

class User {
  public:
    explicit User(const std::string& name, const std::string& password, Group& g);

    User(const User&) = delete;

    ~User();

    void add_group(Group& group);

    void remove_group(const Group& group);

    Group& get_personal_group() const { return personal_group; }

    bool is_in_group(const Group& group) const;

    void clear_groups();

    bool match_password(const std::string& pw) const { return pw == this->password; }

    std::string get_name() const { return this->name; }

    bool is_root() const { return name == "root"; }
  private:
    const std::string name;
    const std::string password;
    std::vector<Group*> other_groups;
    Group& personal_group;
};

#endif // __USER_H_
