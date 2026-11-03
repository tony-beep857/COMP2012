#ifndef __GROUP_H__
#define __GROUP_H__

#include <string>
#include <vector>

class User;

class Group {
  public:
    explicit Group(const std::string& name);

    Group(const Group&) = delete;

    ~Group();

    void add_user(User& user);

    void remove_user(const User& user);

    bool has_user(const User& user) const;

    const std::string& get_name() const {
      return name;
    }
  private:
    const std::string name;
    std::vector<User*> users;
};


#endif // __GROUP_H__
