#include "local_repository.h"
#include "remote_repository.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <system_error>
#include <utility>

#include "util.h"

using std::operator""s;

namespace {
/**
 * @brief The number of hash nibbles before the name itself starts: the sixteen hex digits of a 64-bit FNV-1a hash.
 */
constexpr std::size_t HASH_NIBBLES{16};

/**
 * @brief The bucket that ends a name; the nibbles take buckets 1..16.
 */
constexpr std::size_t END_BUCKET{0};
}  // namespace

LocalRepository::LocalRepository(std::filesystem::path path) :
    path{std::move(path)},
    packages{[](const InstalledPackage& installed) -> std::string { return installed.package.get_name(); },
             LocalRepository::package_bucket} {}

Expected<LocalRepository, std::string>
LocalRepository::load(const std::filesystem::path& index_path,
                      const std::function<const Package*(const std::string&)>& resolve) {
  // An absent index means nothing is installed yet; the file appears on the first
  // save, so a read-only command never creates one.
  std::error_code exists_error;
  const bool present{std::filesystem::exists(index_path, exists_error)};
  if (exists_error) {
    return Unexpected{"Cannot read "s + index_path.string() + ": " + exists_error.message()};
  }

  LocalRepository repository{index_path};
  if (!present) {
    return repository;
  }

  Expected<std::string, std::string> index_text{read_file(index_path)};
  if (!index_text) {
    return Unexpected{index_text.error()};
  }

  const std::string& index{index_text.value()};
  for (std::string_view line : split(index, '\n')) {
    if (!line.empty() && line.back() == '\r') {
      line.remove_suffix(1);
    }
    if (line.empty()) {
      continue;
    }

    std::istringstream fields{std::string{line}};
    std::string name;
    std::string version_text;
    if (!(fields >> name >> version_text)) {
      return Unexpected{
          "Expected '<pkgname> <pkgver> [<explicit>]' in local index line: "s + std::string{line},
      };
    }

    Expected<PkgVersion, std::string> version{PkgVersion::parse(version_text)};
    if (!version) {
      return Unexpected{index_path.string() + ": " + version.error()};
    }

    std::string flag;
    bool explicit_install{false};
    if (fields >> flag) {
      if (flag != "explicit") {
        return Unexpected{"Expected 'explicit' as the third field, got: "s + flag};
      }
      explicit_install = true;
    }

    std::string trailing;
    if (fields >> trailing) {
      return Unexpected{"Unexpected field in local index line: "s + trailing};
    }

    // A name no remote knows keeps its index version and nothing else.
    const Package* resolved{resolve(name)};
    const Package package{resolved != nullptr ? Package{resolved->get_name(),
                                                        resolved->get_desc(),
                                                        version.value(),
                                                        resolved->get_depends(),
                                                        resolved->get_conflicts()}
                                              : Package{
                                                    name,
                                                    "",
                                                    version.value(),
                                                },};
    if (!repository.packages.insert(InstalledPackage{package, explicit_install}).second) {
      return Unexpected{"Duplicate package in local index: "s + name};
    }
  }

  for (const InstalledPackage& installed : repository.packages) {
    for (const std::string& dependency : installed.package.get_depends()) {
      repository.reverse_depends[dependency].push_back(installed.package.get_name());
    }
  }

  return repository;
}

std::optional<std::string> LocalRepository::save(const std::filesystem::path& index_path) const {
  std::vector<const InstalledPackage*> installed;
  installed.reserve(packages.size());
  for (const InstalledPackage& entry : packages) {
    installed.push_back(&entry);
  }
  std::sort(
      installed.begin(), installed.end(), [](const InstalledPackage* left, const InstalledPackage* right) -> bool {
        return left->package.get_name() < right->package.get_name();
      });

  std::ostringstream buffer;
  for (const InstalledPackage* entry : installed) {
    buffer << entry->package.get_name() << ' ' << static_cast<std::string>(entry->package.get_version());
    if (entry->explicit_install) {
      buffer << " explicit";
    }
    buffer << '\n';
  }

  std::filesystem::path temporary{index_path};
  temporary += ".tmp";
  {
    std::ofstream output{temporary, std::ios::binary | std::ios::trunc};
    if (!output) {
      return std::make_optional("Cannot write file: "s + temporary.string());
    }
    output << buffer.str();
    if (!output) {
      return std::make_optional("Cannot write file: "s + temporary.string());
    }
  }

  std::error_code rename_error;
  std::filesystem::rename(temporary, index_path, rename_error);
  if (rename_error) {
    return std::make_optional("Cannot replace "s + index_path.string() + ": " + rename_error.message());
  }
  return std::nullopt;
}

std::size_t LocalRepository::package_bucket(const std::string& name, std::size_t depth) {
  if (depth < HASH_NIBBLES) {
    return nibble(fnv1a(name), depth) + 1;
  }

  const std::size_t offset{depth - HASH_NIBBLES};
  const std::size_t position{offset / 2};
  if (position >= name.size()) {
    return END_BUCKET;
  }
  const unsigned char byte{static_cast<unsigned char>(name[position])};
  return static_cast<std::size_t>((offset % 2 == 0) ? byte >> 4 : byte & 0xF) + 1;
}

Expected<RemoteRepository, std::string> RemoteRepository::open(const std::filesystem::path& index_path) {
  Expected<std::string, std::string> index_text{read_file(index_path)};
  if (!index_text) {
    return Unexpected{index_text.error()};
  }

  const std::filesystem::path dir{index_path.parent_path()};
  const std::string& index{index_text.value()};
  std::vector<Package> loaded;
  for (std::string_view name : split(index, '\n')) {
    if (!name.empty() && name.back() == '\r') {
      name.remove_suffix(1);
    }
    if (name.empty()) {
      continue;
    }

    const std::filesystem::path package_path{dir / std::string{name}};
    Expected<std::string, std::string> package_text{read_file(package_path)};
    if (!package_text) {
      return Unexpected{package_text.error()};
    }

    Expected<Package, std::string> package{Package::parse(package_text.value())};
    if (!package) {
      return Unexpected{package_path.string() + ": " + package.error()};
    }
    loaded.push_back(package.value());
  }

  return RemoteRepository{index_path, std::move(loaded)};
}

std::optional<const Package*> RemoteRepository::search(const std::string& name) const {
  for (const Package& package : packages) {
    if (package.get_name() == name) {
      return &package;
    }
  }
  return std::nullopt;
}

std::vector<const Package*> RemoteRepository::list() const {
  std::vector<const Package*> result;
  result.reserve(packages.size());
  for (const Package& package : packages) {
    result.push_back(&package);
  }
  return result;
}
