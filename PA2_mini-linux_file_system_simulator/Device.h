#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "File.h"

class Device : public File {
  public:
    explicit Device(const std::string& name, const User& owner, const Group& group, Directory* parent) : File{name, owner, group, "", parent} {}
    ~Device() override = default;

    Device(const Device&) = delete;

    char get_type() const final;

  protected:
    bool set_content(const User& user, const std::string& content, const std::string& cmd) override = 0;

    std::optional<std::string> get_content(const User& user, const std::string& cmd) const override = 0;
};


class DevNull final : public Device {
  public:
    explicit DevNull(const User& owner, const Group& group, Directory* parent) : Device{"null", owner, group, parent} {}
    ~DevNull() override = default;

    DevNull(const DevNull&) = delete;

  protected:
    bool set_content(const User& user, const std::string& content, const std::string& cmd) final;

    std::optional<std::string> get_content(const User& user, const std::string& cmd) const final;
};

class DevStdOut final : public Device {
  public:
    explicit DevStdOut(const User& owner, const Group& group, Directory* parent) : Device{"stdout", owner, group, parent} {}
    ~DevStdOut() override = default;

    DevStdOut(const DevStdOut&) = delete;

  protected:
    bool set_content(const User& user, const std::string& content, const std::string& cmd) final;

    std::optional<std::string> get_content(const User& user, const std::string& cmd) const final;
};

class DevStdIn final : public Device {
  public:
    explicit DevStdIn(const User& owner, const Group& group, Directory* parent) : Device{"stdin", owner, group, parent} {}
    ~DevStdIn() override = default;

    DevStdIn(const DevStdIn&) = delete;

  protected:
    bool set_content(const User& user, const std::string& content, const std::string& cmd) final;

    std::optional<std::string> get_content(const User& user, const std::string& cmd) const final;
};

#endif // __DEVICE_H__
