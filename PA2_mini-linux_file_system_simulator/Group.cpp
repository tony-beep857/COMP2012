#include "Group.h"
#include "User.h"

Group::Group(const std::string& name):name(name){}

Group::~Group(){}

void Group::add_user(User& user){
    users.emplace_back(&user);
}

void Group::remove_user(const User& user){
    for (std::vector<User*>::iterator u = users.begin(); u != users.end(); u++ ){
        if(*u == &user){
            users.erase(u);
            break;
        }
    }
}

bool Group::has_user(const User& user) const{
    for (User* u : users){
        if(u == &user){
            return true;
        }
    }
    return false;
}