#include "package_manager.h"

#include <algorithm>
#include <functional>
#include <iterator>
#include <optional>
#include <set>
#include <string>
#include <utility>

bool PackageManager::conflicts(const Package& package, const LocalRepository& repository) {
  // TODO
  for (const std::string& conflict_name : package.get_conflicts()
) {
    if (repository.search(conflict_name).has_value()) {
      return true;
    }
  }

  // Check whether any currently installed package conflicts with `package`
  for (const Package* installed : repository.list()) {
    for (const std::string& conflict_name : installed->get_conflicts()) {
      if (conflict_name == package.get_name()) {
        return true;
      }
    }
  }

  return false;
}

std::optional<std::vector<Package>> PackageManager::collect_missing(const Package& package,
                                                                    const LocalRepository& repository) const {
  // TODO
  std::vector<Package> missing;
  std::set<std::string> seen;
  seen.insert(package.get_name());

  const std::set<std::string>& initial_deps = package.get_depends();
  std::vector<std::string> pending(initial_deps.begin(), initial_deps.end());
  for (std::size_t i = 0; i < pending.size(); ++i) {
    const std::string dep_name = pending[i];

    if (seen.find(dep_name) != seen.end()) {
      continue;
    }

    if (repository.search(dep_name).has_value()) {
      seen.insert(dep_name);
      continue;
    }

    std::optional<const Package*> remote_pkg = remote.search(dep_name);
    if (!remote_pkg.has_value()) {
      return std::nullopt;
    }

    seen.insert(dep_name);
    missing.push_back(**remote_pkg);
    for (const std::string& next_dep : (*remote_pkg)->get_depends()) {
      pending.push_back(next_dep);
    }
  }

  return missing;
}

Status PackageManager::install(const std::string& name, bool explicit_install) {
  // TODO
  if (local.search(name).has_value()) {
    return Status::ALREADY_INSTALLED;
  }

  std::optional<const Package*> target = remote.search(name);
  if (!target.has_value()) {
    return Status::NOT_FOUND;
  }

  std::optional<std::vector<Package>> missing = collect_missing(**target, local);
  if (!missing.has_value()) {
    return Status::NOT_FOUND;
  }

  LocalRepository staged{local};

  if (conflicts(**target, staged)) {
    return Status::CONFLICT;
  }
  staged.put(**target, explicit_install);

  for (const Package& dep : *missing) {
    if (conflicts(dep, staged)) {
      return Status::CONFLICT;
    }
    staged.put(dep, false);
  }

  local = staged;
  return Status::SUCCESS;
}

void PackageManager::remove_with_unused_dependencies(const std::string& name) {
  // TODO
  std::optional<const Package*> pkg = local.search(name);
  if (!pkg.has_value()) {
    return;
  }

  // Copy dependencies before dropping, since local.drop() invalidates *pkg
  std::set<std::string> deps = (*pkg)->get_depends();
  local.drop(name);

  for (const std::string& dep : deps) {
    if (local.search(dep).has_value() &&
        local.is_explicit(dep) == false &&
        local.dependents(dep).empty()) {
      remove_with_unused_dependencies(dep);
    }
  }
}

Status PackageManager::uninstall(const std::string& name) {
  // TODO
  if (!local.search(name).has_value()) {
    return Status::NOT_FOUND;
  }

  if (!local.dependents(name).empty()) {
    return Status::BREAKS_DEPS;
  }

  remove_with_unused_dependencies(name);
  return Status::SUCCESS;
}

Status PackageManager::upgrade() {
  // TODO
  LocalRepository staged{local};
  std::vector<Package> changed;

  // Phase 1: Upgrade eligible packages and install their missing dependencies on `staged`
  for (const Package* cur : local.list()) {
    std::optional<const Package*> remote_pkg = remote.search(cur->get_name());
    if (!remote_pkg.has_value()) {
      // Orphaned package: leave alone
      continue;
    }

    if (!(cur->get_version() < (*remote_pkg)->get_version())) {
      // Remote version is not strictly newer
      continue;
    }

    std::optional<std::vector<Package>> missing = collect_missing(**remote_pkg, staged);
    if (!missing.has_value()) {
      return Status::NOT_FOUND;
    }

    bool was_explicit = local.is_explicit(cur->get_name()).value_or(false);
    staged.put(**remote_pkg, was_explicit);
    changed.push_back(**remote_pkg);

    for (const Package& dep : *missing) {
      staged.put(dep, false);
      changed.push_back(dep);
    }
  }

  // Phase 2: Check conflicts against the final post-upgrade package set
  for (const Package& pkg : changed) {
    if (conflicts(pkg, staged)) {
      return Status::CONFLICT;
    }
  }

  // Phase 3: Commit only after all checks succeed
  local = staged;
  return Status::SUCCESS;
}

std::size_t PackageManager::prune() {
  // TODO
  std::size_t removed{0};

  while (true) {
    std::vector<std::string> to_remove;

    for (const Package* pkg : local.list()) {
      const std::string& name = pkg->get_name();
      if (local.is_explicit(name) == false && local.dependents(name).empty()) {
        to_remove.push_back(name);
      }
    }

    if (to_remove.empty()) {
      break;
    }

    for (const std::string& name : to_remove) {
      local.drop(name);
      ++removed;
    }
  }

  return removed;
}

std::vector<std::string> PackageManager::orphans() const {
  // TODO
  std::vector<std::string> result;

  for (const Package* pkg : local.list()) {
    if (!remote.search(pkg->get_name()).has_value()) {
      result.push_back(pkg->get_name());
    }
  }

  std::sort(result.begin(), result.end());
  return result;
}
