#ifndef __FILE_ENTRY_H__
#define __FILE_ENTRY_H__

#include <optional>
#include <string>

#include "Group.h"
#include "User.h"

class Directory;
class File;

struct FormatData {
  std::string::size_type max_owner;
  std::string::size_type max_group;
  std::string::size_type max_size;
};

enum Permission {
  Exe = 1,
  Write = 2,
  Read = 4,
};

class FileEntry {
public:
  explicit FileEntry(const std::string& name, const User& owner, const Group& group, Directory* parent, unsigned short permissions = 0755);

  FileEntry(const FileEntry&) = delete;

  virtual ~FileEntry() = default;

  virtual unsigned int get_size() const = 0;

  virtual char get_type() const = 0;

  virtual bool rm(const User& user) = 0;

  const std::string& get_name() const { return name; }

  const User* get_owner() const { return owner; }

  const Group* get_group() const { return group; }

  const Directory* get_parent() const { return parent; }

  Directory* get_parent() { return parent; }

  virtual void cat(const User& user) const = 0;

  virtual bool ls(const User& user, bool all, bool recursive, const std::optional<FormatData>& long_format, const std::string& display_name = "") const;

  virtual bool chmod(const User& user, const std::string &new_perm, bool recursive);

  virtual bool cp(const User& user, FileEntry& dest, bool preserve_perms) = 0;

  std::string get_full_path() const;

  void copy_permission(const FileEntry& other) { permissions = other.permissions; }
private:
  const std::string name;
  const User* owner;
  const Group* group;
  Directory* parent;
  unsigned short permissions;

  std::optional<unsigned short> parse_permission(const std::string &perm) const;

protected:
  bool has_permissions(const User& user, Permission perm, const std::string& cmd) const;

  bool is_owned_by(const User& user, const std::string& cmd) const;
};

#endif // __FILE_ENTRY_H__
