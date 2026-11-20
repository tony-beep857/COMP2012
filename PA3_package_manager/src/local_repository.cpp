#include "local_repository.h"

#include <algorithm>
#include <utility>

std::optional<const Package*> LocalRepository::search(const std::string& name) const {
  // TODO
  Trie<InstalledPackage, std::string, 17>::const_iterator it = packages.find(name);
  if (it == packages.end()) {
    return std::nullopt;
  }
  return &((*it).package);
}

std::vector<const Package*> LocalRepository::list() const {
  // TODO
  std::vector<const Package*> result;
  result.reserve(packages.size());
  for (const InstalledPackage& entry : packages) {
    result.push_back(&entry.package);
  }
  return result;
}

void LocalRepository::put(const Package& package, bool explicit_install) {
  // TODO
  InstalledPackage entry{package, explicit_install};
  const std::string name = entry.package.get_name();

  std::optional<const Package*> existing = search(name);
  if (existing.has_value()) {
    for (const std::string& dep : (*existing)->get_depends()) {
      unlink(dep, name);
    }
    packages.erase(name);
  }

  for (const std::string& dep : entry.package.get_depends()) {
    std::vector<std::string>& dependents = reverse_depends[dep];
    if (std::find(dependents.begin(), dependents.end(), name) == dependents.end()) {
      dependents.push_back(name);
    }
  }

  packages.insert(entry);
}

void LocalRepository::drop(const std::string& name) {
  // TODO
const std::string target = name;
  std::optional<const Package*> existing = search(target);
  if (!existing.has_value()) {
    return;
  }

  for (const std::string& dep : (*existing)->get_depends()) {
    unlink(dep, target);
  }

  reverse_depends.erase(target);
  packages.erase(target);
}

void LocalRepository::unlink(const std::string& dependency, const std::string& dependent) {
  // TODO
  std::map<std::string, std::vector<std::string>>::iterator it = reverse_depends.find(dependency);
  if (it == reverse_depends.end()) {
    return;
  }

  std::vector<std::string>& dependents = it->second;
  dependents.erase(
      std::remove(dependents.begin(), dependents.end(), dependent),
      dependents.end());

  if (dependents.empty()) {
    reverse_depends.erase(it);
  }
}

unsigned long long LocalRepository::fnv1a(const std::string& text) {
  constexpr unsigned long long FNV_OFFSET_BASIS{14695981039346656037ULL};
  constexpr unsigned long long FNV_PRIME{1099511628211ULL};

  // TODO
  unsigned long long hash{FNV_OFFSET_BASIS};
  for (char c : text) {
    hash ^= static_cast<unsigned char>(c);
    hash *= FNV_PRIME;
  }
  return hash;
}
