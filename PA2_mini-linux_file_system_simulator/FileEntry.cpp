#include "FileEntry.h"
#include "Diagnose.h"

FileEntry::FileEntry(const std::string &name, const User& owner, const Group& group, Directory *parent, unsigned short permissions):
    name(name), owner(&owner), group(&group), parent(parent), permissions(permissions){}

bool FileEntry::is_owned_by(const User& user, const std::string& cmd) const{
    if(user.is_root() || &user == owner)
        return true;
    diagnose::report(cmd, get_name(), diagnose::PermissionDenied);
    return false;
}

bool FileEntry::chmod(const User& user, const std::string &new_perm, bool recursive){
    std::optional<unsigned short> new_permission = parse_permission(new_perm);
    if (new_permission.has_value()){
        permissions = new_permission.value();
        return true;
    }
    return false;
}