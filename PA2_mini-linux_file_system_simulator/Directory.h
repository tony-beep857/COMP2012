#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#include <string>
#include <vector>

#include "FileEntry.h"
#include "FileSystem.h"

using path_t = std::vector<std::string>;
using entry_vec_t = std::vector<FileEntry*>;

enum SearchOption {
  Error,
  CreateFileIfNotExists,
  CreateDirIfNotExists,
};

class Directory final : public FileEntry {
  public:
    explicit Directory(const std::string& name, const User& owner, const Group& group, Directory* parent);

    Directory(const Directory&) = delete;

    ~Directory() final;

    bool insert_entry(const User& user, FileEntry* entry, const std::string& cmd);

    bool remove_entry(const User& user, const FileEntry& entry, const std::string& cmd);

    bool is_ancestor_of(const Directory& other) const;

    bool rm(const User& user) final;

    FileEntry* search(const User& user, const std::string& target, const std::string& cmd, SearchOption option = SearchOption::Error);

    unsigned int get_size() const final;

    unsigned int get_entry_count() const { return file_entries.size(); }

    FormatData compute_ls_widths(bool all, bool recursive) const;

    char get_type() const final;

    void cat(const User& user) const final;

    bool ls(const User& user, bool all, bool recursive, const std::optional<FormatData>& long_format, const std::string& display_name = "") const final;

    bool chmod(const User& user, const std::string &new_perm, bool recursive) final;

    bool cp(const User& user, FileEntry& dest, bool preserve_perms) final;
  private:
    entry_vec_t file_entries;
};

#endif // __DIRECTORY_H__
