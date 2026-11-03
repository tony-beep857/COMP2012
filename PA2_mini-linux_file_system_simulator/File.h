#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <optional>

#include "FileEntry.h"
#include "User.h"

class File : public FileEntry {
  public:
    explicit File(const std::string& name, const User& owner, const Group& group, const std::string& content, Directory *parent);

    File(const File&) = delete;

    ~File() override = default;

    unsigned int get_size() const final;

    char get_type() const override;

    void cat(const User& user) const final;

    bool ls(const User& user, bool all, bool recursive, const std::optional<FormatData>& long_format, const std::string& display_name = "") const final;

    bool chmod(const User& user, const std::string &new_perm, bool recursive) final;

    bool cp(const User& user, FileEntry& dest, bool preserve_perms) final;

    bool rm(const User& user) final;

  protected:
    virtual bool set_content(const User& user, const std::string& content, const std::string& cmd);

    virtual std::optional<std::string> get_content(const User& user, const std::string& cmd) const;

  private:
    std::string content;
};

#endif // __FILE_H__
