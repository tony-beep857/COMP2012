#include "FileSystem.h"

#include "Device.h"
#include "Directory.h"
#include <iostream>

namespace path {
  path_t split_path_string(const std::string& str) {
    path_t path;
    std::string::size_type start = 0, end;
    while ((end = str.find('/', start)) != std::string::npos) {
      path.emplace_back(str.substr(start, end - start));
      start = end + 1;
    }

    if (start < str.size()) {
      path.emplace_back(str.substr(start));
    }

    return path;
  }
}

FileSystem::FileSystem(const User& root_user) : root{nullptr}, cwd{nullptr} {
  const Group& root_group = root_user.get_personal_group();

  root = new Directory{"", root_user, root_group, nullptr};

  root->insert_entry(root_user, new Directory{"home", root_user, root_group, root}, "");


  cwd = new Directory{"root", root_user, root_group, root};
  root->insert_entry(root_user, cwd, "");
  Directory* dev_dir = new Directory{"dev", root_user, root_group, root};
  root->insert_entry(root_user, dev_dir, "");
  DevNull* dev_null = new DevNull{root_user, root_group, dev_dir};
  dev_dir->insert_entry(root_user, dev_null, "");
  DevStdOut* dev_out = new DevStdOut{root_user, root_group, dev_dir};
  dev_dir->insert_entry(root_user, dev_out, "");
  DevStdIn* dev_in = new DevStdIn{root_user, root_group, dev_dir};
  dev_dir->insert_entry(root_user, dev_in, "");
}

std::string FileSystem::pwd() const {
  std::string p = cwd->get_full_path();
  return p.empty() ? "/" : p;
}

std::string FileEntry::get_full_path() const {
  if (parent == nullptr) {
    return name;
  }

  return parent->get_full_path() + "/" + name;
}
