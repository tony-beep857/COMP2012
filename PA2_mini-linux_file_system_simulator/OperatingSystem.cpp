#include "OperatingSystem.h"
#include "FileSystem.h"
#include "Diagnose.h"

OperatingSystem::OperatingSystem():cur_user(nullptr){
    Group* root_group = new Group("root");
    User* root_user = new User("root", "root", *root_group);
    
    root_group->add_user(*root_user);
    groups.emplace_back(root_group);
    users.emplace_back(root_user);

    filesys = new FileSystem(*root_user);
}

OperatingSystem::~OperatingSystem(){
    delete filesys;

    for (User* user : users){
        delete user;
    }

    for (Group* group : groups){
        delete group;
    }
}

User* OperatingSystem::search_user(const std::string& name) const{
    for(User* user : users){
        if(user->get_name() == name){
            return user;
        }
    }
    return nullptr;
}

Group* OperatingSystem::search_group(const std::string& name) const{
    for(Group* group : groups){
        if(group->get_name() == name){
            return group;
        }
    }
    return nullptr;
}

void OperatingSystem::groupadd(const std::string& group_name){
    if (cur_user->is_root() && search_group(group_name) == nullptr ){
        Group* new_group = new Group(group_name);
        groups.emplace_back(new_group);
    }
}//不能确定是否需要报错

void OperatingSystem::usermod(const std::string& user, const std::vector<std::string>& groups, bool append){
//remember to report the error
    if(!cur_user->is_root()){
        diagnose::report("usermod", user, diagnose::PermissionDenied);//报错信息不确定
        return;
    }
    User* target_user = search_user(user);
    if( target_user == nullptr){
        diagnose::report("usermod", user, diagnose::NoUserOrGroup);
        return;
    }
    if(!append){ //append = false
        target_user->clear_groups();
    }
    for (std::string group_name : groups){
        Group* group = search_group(group_name);
        if (group == nullptr){
            diagnose::report("usermod", group_name, diagnose::NoUserOrGroup);
            return;
        }
        group->add_user(*target_user);
        target_user->add_group(*group);
    }
}