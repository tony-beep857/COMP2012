#include "Directory.h"

#include "Diagnose.h"

using diagnose::report;
using diagnose::ErrorType;

FormatData Directory::compute_ls_widths(bool all, bool recursive) const {
  FormatData data{0, 0, 0};
  if (all) {
    // Handle .
    std::string::size_type ow = get_owner()->get_name().size();
    std::string::size_type gw = get_group()->get_name().size();
    std::string::size_type sw = std::to_string(get_size()).size();
    data.max_owner = std::max(data.max_owner, ow);
    data.max_group = std::max(data.max_group, gw);
    data.max_size = std::max(data.max_size, sw);

    // Handle .. (we need to account for root)
    const Directory* dotdot = get_parent() ? get_parent() : this;
    ow = dotdot->get_owner()->get_name().size();
    gw = dotdot->get_group()->get_name().size();
    sw = std::to_string(dotdot->get_size()).size();
    data.max_owner = std::max(data.max_owner, ow);
    data.max_group = std::max(data.max_group, gw);
    data.max_size = std::max(data.max_size, sw);
  }

  for (FileEntry* entry : file_entries) {
    if (!all && !entry->get_name().empty() && entry->get_name()[0] == '.') continue;
    std::string::size_type ow = entry->get_owner()->get_name().size();
    std::string::size_type gw = entry->get_group()->get_name().size();
    std::string::size_type sw = std::to_string(entry->get_size()).size();
    data.max_owner = std::max(data.max_owner, ow);
    data.max_group = std::max(data.max_group, gw);
    data.max_size = std::max(data.max_size, sw);

    if (recursive) {
      Directory* d = dynamic_cast<Directory*>(entry);
      if (d != nullptr) {
        FormatData sub_data = d->compute_ls_widths(all, recursive);
        data.max_owner = std::max(data.max_owner, sub_data.max_owner);
        data.max_group = std::max(data.max_group, sub_data.max_group);
        data.max_size = std::max(data.max_size, sub_data.max_size);
      }
    }
  }

  return data;
}

bool Directory::is_ancestor_of(const Directory& other) const {
  if (other.get_full_path() == get_full_path()) {
    return true;
  }
  const Directory* p = other.get_parent();
  if (p != nullptr) {
    return is_ancestor_of(*p);
  }
  else {
    return false;
  }
}
