#include "User.h"
#include "Group.h"

User::User(const std::string& name, const std::string& password, Group& g):
    name(name), password(password), personal_group(g){}

User::~User(){}

void User::add_group(Group& group){
    other_groups.emplace_back(&group);
}

void User::remove_group(const Group& group){
    for (std::vector<Group*>::iterator i = other_groups.begin(); i != other_groups.end(); i++){
        if (*i == &group){
            other_groups.erase(i);
            break;
        }
    }
}

bool User::is_in_group(const Group& group) const{
    if(&personal_group == & group)
        return true;
    return group.has_user(*this);
}

void User::clear_groups(){
    for (Group* group : other_groups){
        group->remove_user(*this);
    }
    other_groups.clear();
}