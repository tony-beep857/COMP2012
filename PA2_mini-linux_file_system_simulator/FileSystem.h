#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <string>
#include <vector>

#include "FileEntry.h"
#include "Group.h"
#include "User.h"

using path_t = std::vector<std::string>;

namespace path {
  path_t split_path_string(const std::string& str);
} // namespace path

class Directory;

class FileSystem {
  public:
    explicit FileSystem(const User& root_user);

    ~FileSystem();

    FileSystem(const FileSystem&) = delete;

    Directory* get_root() const { return root; }

    std::string pwd() const;

    void cd(const User& user, const std::string& target);

    void cat(const User& user, const std::string& target) const;

    void chmod(const User& user, const std::string& target, const std::string& new_perm, bool recursive) const;
    void cp(const User& user, const std::string& src_path, const std::string& dest_path, bool recursive, bool preserve_perms) const;
    void ls(const User& user, const path_t& targets, bool all, bool long_format, bool recursive) const;

    void mkdir(const User& user, const path_t& targets, bool create_parents) const;
    void touch(const User& user, const path_t& targets) const;

    void rm(const User& user, const path_t& targets, bool recursive) const;

    void add_user_dir(const User& root, const User& user, const std::string& cmd);
  private:
    Directory* root;
    Directory* cwd;

    FileEntry* resolve_path(const User& user, const path_t& path, const std::string& cmd) const;
    Directory* resolve_dir(const User& user, const path_t& path, const std::string& cmd) const;
};

#endif // __FILESYSTEM_H__
