#include "package_manager.h"

#include <filesystem>
#include <functional>
#include <string>

Expected<PackageManager, std::string> PackageManager::create(const std::filesystem::path& local_index,
                                                             const std::filesystem::path& remote_index) {
  Expected<RemoteRepository, std::string> remote{RemoteRepository::open(remote_index)};
  if (!remote) {
    return Unexpected{remote.error()};
  }

  const RemoteRepository& opened{remote.value()};
  const std::function<const Package*(const std::string&)> resolve{[&opened](const std::string& name) -> const Package* {
    const std::optional<const Package*> found{opened.search(name)};
    return found ? *found : nullptr;
  }};

  Expected<LocalRepository, std::string> local{LocalRepository::load(local_index, resolve)};
  if (!local) {
    return Unexpected{local.error()};
  }

  return PackageManager{local.value(), remote.value()};
}
