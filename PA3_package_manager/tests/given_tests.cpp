#include "given_tests.h"

#include "fixtures.h"
#include "test.h"

#include "local_repository.h"
#include "package.h"
#include "package_manager.h"
#include "trie.h"
#include "version.h"

#include <cassert>
#include <cstddef>
#include <filesystem>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {
// NOLINTBEGIN(misc-static-assert)

/**
 * @brief A `Trie<int, int, 10>` keyed by value and bucketed by decimal digit: bucket `depth` is the digit `depth`
 * places from the right.
 */
struct DenaryIntTrie : public TrieSpecialization<int, int, 10> {
  DenaryIntTrie() : TrieSpecialization{key_fn, bucket_fn} {}

  /**
   * @brief Reads the key of an `int` trie value: the value itself.
   *
   * @param val The value.
   * @return The value, used as its own key.
   */
  static int key_fn(const int& val) { return val; }

  /**
   * @brief Routes an `int` key by decimal digit: bucket `depth` is the digit `depth` places from the right.
   *
   * @param val The key.
   * @param depth The depth in the trie.
   * @return The bucket index, in `[0, SPAN)`.
   */
  static std::size_t bucket_fn(const int& val, std::size_t depth) {
    int value = val;
    for (std::size_t d = 0; d < depth; ++d) {
      value /= static_cast<int>(SPAN);
    }
    return static_cast<std::size_t>(value) % SPAN;
  }
};

/**
 * @brief Checks that an empty trie holds nothing, and says so three ways.
 */
void GivenTestSuite_Trie_Empty() {
  DenaryIntTrie trie;
  assert(trie->size() == 0 && "a fresh trie has no values");  // NOLINT(readability-container-size-empty)
  assert(trie->empty() && "a fresh trie reports empty");
  assert(trie->begin() == trie->end() && "a fresh trie has no elements");
}

/**
 * @brief Checks that inserting a new key reports that it was new, and hands back the stored value.
 */
void GivenTestSuite_Trie_Insert_ReportsNew() {
  DenaryIntTrie trie;
  const std::pair<DenaryIntTrie::iterator, bool> result = trie->insert(5);

  assert(result.second && "insert of a new key reports the value was stored");
  assert(*result.first == 5 && "the returned iterator points at the stored value");
}

/**
 * @brief Checks that inserting a new key stores it, making it counted and findable.
 */
void GivenTestSuite_Trie_Insert_Stores() {
  DenaryIntTrie trie;
  trie->insert(5);

  assert(trie->size() == 1 && "one insert stores one value");
  assert(trie->find(5) != trie->end() && "the stored value is findable");
}

/**
 * @brief Checks that keys landing in distinct first buckets stay distinct.
 */
void GivenTestSuite_Trie_Insert_DistinctBuckets() {
  DenaryIntTrie trie;
  for (const int value : {0, 1, 2, 3, 9}) {
    assert(trie->insert(value).second && "insert of a single-digit key failed");
  }

  assert(trie->size() == 5 && "five single-digit keys are five values");
  assert(as_set(trie.collect()) == std::set<int>({0, 1, 2, 3, 9}) && "every single-digit key is stored");
}

/**
 * @brief Checks that keys sharing a prefix route stay distinct at the depth they diverge.
 */
void GivenTestSuite_Trie_Insert_SharedPrefix() {
  DenaryIntTrie trie;
  for (const int value : {12, 102, 9}) {
    assert(trie->insert(value).second && "insert of a multi-digit key failed");
  }

  assert(trie->size() == 3 && "three keys that share a prefix are three values");
  assert(as_set(trie.collect()) == std::set<int>({12, 102, 9}) && "every key on a shared route is stored");
}

/**
 * @brief Checks that the iterator `insert` returns walks from the inserted key to the end.
 */
void GivenTestSuite_Trie_Insert_ReturnsIterator() {
  DenaryIntTrie trie;
  for (const int value : {0, 1, 2, 9}) {
    trie->insert(value);
  }

  const std::pair<DenaryIntTrie::iterator, bool> result = trie->insert(5);
  assert(result.second && "insert of 5 failed");

  std::vector<int> remaining;
  for (DenaryIntTrie::iterator it = result.first; it != trie->end(); ++it) {
    remaining.push_back(*it);
  }
  assert((remaining == std::vector<int>{5, 9}) && "the iterator returned by insert does not walk the trie");
}

/**
 * @brief Checks that `find` reports a stored key.
 */
void GivenTestSuite_Trie_Find_Present() {
  DenaryIntTrie trie;
  trie->insert(5);

  assert(trie->find(5) != trie->end() && "find of a present key returned end()");
  assert(*trie->find(5) == 5 && "find returned the wrong element");
}

/**
 * @brief Checks that `find` reports an absent key as `end()`.
 */
void GivenTestSuite_Trie_Find_Absent() {
  DenaryIntTrie trie;
  trie->insert(5);
  assert(trie->find(7) == trie->end() && "find of an absent key returned an element");

  DenaryIntTrie empty;
  assert(empty->find(5) == empty->end() && "find of a key that was never inserted returned an element");
}

/**
 * @brief Checks that erasing a stored key removes it and reports one removal.
 */
void GivenTestSuite_Trie_Erase() {
  DenaryIntTrie trie;
  trie->insert(5);

  assert(trie->erase(5) == 1 && "erase of a present key returned the wrong count");
  // NOLINTNEXTLINE(readability-container-size-empty)
  assert(trie->size() == 0 && trie->begin() == trie->end() && "erase left the trie non-empty");
  assert(trie->find(5) == trie->end() && "erase left the key findable");
}

/**
 * @brief Checks that erasing an absent key reports no removal and changes nothing.
 */
void GivenTestSuite_Trie_Erase_Absent() {
  DenaryIntTrie trie;
  trie->insert(5);

  assert(trie->erase(7) == 0 && "erase of an absent key returned a nonzero count");
  assert(trie->size() == 1 && trie->find(5) != trie->end() && "erase of an absent key changed the trie");
}

/**
 * @brief Checks that a copy has the same values as its original.
 */
void GivenTestSuite_Trie_CopyConstructor() {
  DenaryIntTrie original;
  for (const int value : {12, 102, 9}) {
    original->insert(value);
  }

  const DenaryIntTrie copy_constructed{original};
  assert(copy_constructed->size() == 3 && "the copy has the same size");
  assert(copy_constructed.collect() == original.collect() && "the copy has the same values");
}

/**
 * @brief Checks that assignment replaces the target's values with the source's.
 */
void GivenTestSuite_Trie_CopyAssignment() {
  DenaryIntTrie original;
  for (const int value : {12, 102, 9}) {
    original->insert(value);
  }

  DenaryIntTrie copy_assigned;
  copy_assigned->insert(9);
  copy_assigned = original;
  assert(copy_assigned->size() == 3 && "assignment did not replace the old contents");
  assert(copy_assigned.collect() == original.collect() && "assignment did not copy the values");
}

/**
 * @brief Checks that iteration walks the values in bucket order.
 */
void GivenTestSuite_Trie_Iterate() {
  DenaryIntTrie trie;
  for (const int value : {12, 102, 9}) {
    trie->insert(value);
  }

  // 102 and 12 share bucket 2 at depth 0; 102's second digit is 0, so it comes first.
  assert((trie.collect() == std::vector<int>{102, 12, 9}) && "iteration does not follow the bucket order");
}

/**
 * @brief Checks that the const `begin`/`end` overloads walk the same values as the non-const ones.
 */
void GivenTestSuite_Trie_Iterate_Const() {
  DenaryIntTrie trie;
  for (const int value : {12, 102, 9}) {
    trie->insert(value);
  }

  const DenaryIntTrie& view = trie;
  assert((view.collect() == std::vector<int>{102, 12, 9}) && "const begin/end did not walk every value in order");
}

/**
 * @brief Checks that `cbegin`/`cend` walk every value in order, starting at the first one.
 */
void GivenTestSuite_Trie_Cbegin() {
  DenaryIntTrie trie;
  for (const int value : {12, 102, 9}) {
    trie->insert(value);
  }

  std::vector<int> via_cbegin;
  for (DenaryIntTrie::const_iterator it = trie->cbegin(); it != trie->cend(); ++it) {
    via_cbegin.push_back(*it);
  }
  assert((via_cbegin == std::vector<int>{102, 12, 9}) && "cbegin/cend did not walk every value in order");
  assert(*trie->cbegin() == 102 && "cbegin is not the first value");
}

/**
 * @brief Checks that `find` on a const trie reports a stored key and an absent key as `end()`.
 */
void GivenTestSuite_Trie_Find_Const() {
  DenaryIntTrie trie;
  trie->insert(102);

  const DenaryIntTrie& view = trie;
  assert(view->find(102) != view->end() && *view->find(102) == 102 && "const find of a present key failed");
  assert(view->find(3) == view->end() && "const find of an absent key returned an element");
}

/**
 * @brief Checks that clearing an empty trie is a no-op that leaves it usable.
 */
void GivenTestSuite_Trie_Clear_Empty() {
  DenaryIntTrie trie;
  trie->clear();
  assert(trie->empty() && "clearing an empty trie left it non-empty");
  assert(trie->insert(1).second && "the trie is not usable after clearing it while empty");
}

/**
 * @brief Checks that `clear` empties a populated trie and leaves it reusable.
 */
void GivenTestSuite_Trie_Clear_Populated() {
  DenaryIntTrie trie;
  trie->insert(5);
  trie->insert(12);

  trie->clear();
  assert(trie->size() == 0 && "clear left values behind");  // NOLINT(readability-container-size-empty)
  assert(trie->begin() == trie->end() && "clear left an iterator behind");
  assert(trie->insert(7).second && *trie->find(7) == 7 && "the trie is not reusable after clear");
}

/**
 * @brief Checks the version ordering: major, then minor, then patch.
 */
void GivenTestSuite_PkgVersion_Ordering() {
  const PkgVersion patch = PkgVersion::parse("1.2.3").value();
  const PkgVersion next_patch = PkgVersion::parse("1.2.4").value();
  const PkgVersion next_minor = PkgVersion::parse("1.3.0").value();
  const PkgVersion next_major = PkgVersion::parse("2.0.0").value();

  assert(patch < next_patch && "patch orders");
  assert(next_patch < next_minor && "minor outranks patch");
  assert(next_minor < next_major && "major outranks minor");
  assert(!(next_major < patch) && "a larger version is not less");
}

/**
 * @brief Checks that `==` compares all three components.
 */
void GivenTestSuite_PkgVersion_Equality() {
  const PkgVersion patch = PkgVersion::parse("1.2.3").value();
  const PkgVersion next_patch = PkgVersion::parse("1.2.4").value();

  assert(patch == PkgVersion::parse("1.2.3").value() && "equal versions compare equal");
  assert(!(patch == next_patch) && "different versions do not compare equal");
}

/**
 * @brief Checks that the string conversion writes the dotted form.
 */
void GivenTestSuite_PkgVersion_ToString() {
  const PkgVersion version = PkgVersion::parse("1.2.3").value();
  assert(static_cast<std::string>(version) == "1.2.3" && "the string conversion is dotted");
}

/**
 * @brief Checks that the stream operator writes the dotted form.
 */
void GivenTestSuite_PkgVersion_StreamOperator() {
  const PkgVersion version = PkgVersion::parse("1.2.3").value();

  std::ostringstream stream;
  stream << version;
  assert(stream.str() == "1.2.3" && "operator<< writes the dotted form");
}

/**
 * @brief Checks that install stores the package it was asked for.
 */
void GivenTestSuite_PackageManager_Install() {
  const std::filesystem::path root = case_root("given");
  std::filesystem::create_directories(root / "remote");
  write_package(root / "remote", "meta", "1.0.0", "helper");
  write_package(root / "remote", "helper", "1.0.0");
  const std::filesystem::path remote_index = write_remote(root / "remote", {"meta", "helper"});
  write_local_index(root / "local.index", {});

  Expected<PackageManager, std::string> manager = PackageManager::create(root / "local.index", remote_index);
  assert(manager && "the manager loads");
  assert(manager->install("meta", true) == Status::SUCCESS && "install succeeds");
  assert(manager->get_local().search("meta").has_value() && "the package is installed");
}

/**
 * @brief Checks that install pulls in the package's dependencies as non-explicit packages.
 */
void GivenTestSuite_PackageManager_Install_Dependencies() {
  const std::filesystem::path root = case_root("given");
  std::filesystem::create_directories(root / "remote");
  write_package(root / "remote", "meta", "1.0.0", "helper");
  write_package(root / "remote", "helper", "1.0.0");
  const std::filesystem::path remote_index = write_remote(root / "remote", {"meta", "helper"});
  write_local_index(root / "local.index", {});

  Expected<PackageManager, std::string> manager = PackageManager::create(root / "local.index", remote_index);
  assert(manager && "the manager loads");
  assert(manager->install("meta", true) == Status::SUCCESS && "install succeeds");
  assert(manager->get_local().search("helper").has_value() && "the dependency is installed");
  assert(manager->get_local().is_explicit("helper") == false && "the dependency is not explicit");
}

/**
 * @brief Checks that installing a package that is already installed is refused.
 */
void GivenTestSuite_PackageManager_Install_AlreadyInstalled() {
  const std::filesystem::path root = case_root("given");
  const std::filesystem::path remote_index = write_standard_remote(root / "remote");
  write_local_index(root / "local.index", {});

  Expected<PackageManager, std::string> manager = PackageManager::create(root / "local.index", remote_index);
  assert(manager && "the manager loads");
  assert(manager->install("numcpp", true) == Status::SUCCESS && "the first install succeeds");
  assert(manager->install("numcpp", true) == Status::ALREADY_INSTALLED && "a second install is refused");
}

/**
 * @brief Checks that installing a package no remote provides reports it as not found.
 */
void GivenTestSuite_PackageManager_Install_NotFound() {
  const std::filesystem::path root = case_root("given");
  const std::filesystem::path remote_index = write_standard_remote(root / "remote");
  write_local_index(root / "local.index", {});

  Expected<PackageManager, std::string> manager = PackageManager::create(root / "local.index", remote_index);
  assert(manager && "the manager loads");
  assert(manager->install("ghost", true) == Status::NOT_FOUND && "an unknown package is not found");
}

/**
 * @brief Checks that uninstall removes an explicitly installed package.
 */
void GivenTestSuite_PackageManager_Uninstall() {
  const std::filesystem::path root = case_root("given");
  const std::filesystem::path remote_index = write_standard_remote(root / "remote");
  write_local_index(root / "local.index", {});

  Expected<PackageManager, std::string> manager = PackageManager::create(root / "local.index", remote_index);
  assert(manager && "the manager loads");
  assert(manager->install("numcpp", true) == Status::SUCCESS && "numcpp installs");
  assert(manager->get_local().search("numcpp").has_value() && "numcpp is installed before the uninstall");
  assert(manager->uninstall("numcpp") == Status::SUCCESS && "uninstall succeeds");
  assert(!manager->get_local().search("numcpp") && "the package is gone");
}

/**
 * @brief Checks that upgrade moves an installed package to the remote's version.
 */
void GivenTestSuite_PackageManager_Upgrade_Version() {
  const std::filesystem::path root = case_root("given");
  const std::filesystem::path remote_index = write_standard_remote(root / "remote");
  write_local_index(root / "local.index", {"numcpp 0.9.0 explicit"});

  Expected<PackageManager, std::string> manager = PackageManager::create(root / "local.index", remote_index);
  assert(manager && "the manager loads");
  assert(manager->upgrade() == Status::SUCCESS && "upgrade succeeds");
  const std::optional<const Package*> numcpp = manager->get_local().search("numcpp");
  assert(numcpp && "numcpp is still installed");
  assert(static_cast<std::string>((*numcpp)->get_version()) == "1.0.0" && "the version is bumped");
}

/**
 * @brief Checks that upgrade keeps the installation flag of an explicit package.
 */
void GivenTestSuite_PackageManager_Upgrade_KeepsExplicit() {
  const std::filesystem::path root = case_root("given");
  const std::filesystem::path remote_index = write_standard_remote(root / "remote");
  write_local_index(root / "local.index", {"numcpp 0.9.0 explicit"});

  Expected<PackageManager, std::string> manager = PackageManager::create(root / "local.index", remote_index);
  assert(manager && "the manager loads");
  assert(manager->upgrade() == Status::SUCCESS && "upgrade succeeds");
  assert(static_cast<std::string>(manager->get_local().search("numcpp").value()->get_version()) == "1.0.0"
         && "the upgrade replaced numcpp");
  assert(manager->get_local().is_explicit("numcpp") == true && "the installation flag is kept");
}

/**
 * @brief Checks that prune removes a package that nothing depends on and that nobody installed.
 */
void GivenTestSuite_PackageManager_Prune() {
  const std::filesystem::path root = case_root("given");
  const std::filesystem::path remote_index = write_standard_remote(root / "remote");
  write_local_index(root / "local.index", {"octplotlib 1.0.0"});

  Expected<PackageManager, std::string> manager = PackageManager::create(root / "local.index", remote_index);
  assert(manager && "the manager loads");
  assert(manager->prune() == 1 && "prune removes the unused dependency");
  assert(manager->get_local().list().empty() && "the local repository is empty");
}

/**
 * @brief Checks that orphans lists the packages that no remote provides, sorted.
 */
void GivenTestSuite_PackageManager_Orphans() {
  const std::filesystem::path root = case_root("given");
  std::filesystem::create_directories(root / "remote");
  write_package(root / "remote", "alpha", "1.0.0");
  const std::filesystem::path remote_index = write_remote(root / "remote", {"alpha"});
  write_local_index(root / "local.index", {"zeta 1.0.0 explicit", "alpha 1.0.0 explicit", "beta 1.0.0"});

  Expected<PackageManager, std::string> manager = PackageManager::create(root / "local.index", remote_index);
  assert(manager && "the manager loads");
  assert((manager->orphans() == std::vector<std::string>{"beta", "zeta"})
         && "orphans are sorted and skip provided names");
}

/**
 * @brief Checks that `put` stores a package's installation flag.
 */
void GivenTestSuite_LocalRepository_Put_Flag() {
  const std::filesystem::path root = case_root("given");
  Expected<LocalRepository, std::string> repository =
      LocalRepository::load(root / "local.index", [](std::string_view) -> const Package* { return nullptr; });
  assert(repository && "the empty repository loads");

  repository->put(Package{"alpha", "d", PkgVersion::parse("1.0.0").value(), {"dep"}, {}}, true);
  assert(repository->is_explicit("alpha") == true && "put stores the flag");
}

/**
 * @brief Checks that `put` records the package as a dependent of each dependency.
 */
void GivenTestSuite_LocalRepository_Put_Dependents() {
  const std::filesystem::path root = case_root("given");
  Expected<LocalRepository, std::string> repository =
      LocalRepository::load(root / "local.index", [](std::string_view) -> const Package* { return nullptr; });
  assert(repository && "the empty repository loads");

  repository->put(Package{"alpha", "d", PkgVersion::parse("1.0.0").value(), {"dep"}, {}}, true);
  assert(repository->dependents("dep") == std::vector<std::string>{"alpha"} && "put records the reverse dependency");
}

/**
 * @brief Checks that `put` replaces a package's flag, version and reverse dependencies under the same name.
 */
void GivenTestSuite_LocalRepository_Put_Replaces() {
  const std::filesystem::path root = case_root("given");
  Expected<LocalRepository, std::string> repository =
      LocalRepository::load(root / "local.index", [](std::string_view) -> const Package* { return nullptr; });
  assert(repository && "the empty repository loads");

  repository->put(Package{"alpha", "d", PkgVersion::parse("1.0.0").value(), {"dep"}, {}}, true);

  const Package replacement{"alpha", "d2", PkgVersion::parse("2.0.0").value(), {}, {}};
  repository->put(replacement, false);
  assert(repository->is_explicit("alpha") == false && "put replaces the flag");
  assert(repository->dependents("dep").empty() && "replacing drops the old reverse dependency");
  assert(static_cast<std::string>(repository->search("alpha").value()->get_version()) == "2.0.0"
         && "replacing changes the version");
}

/**
 * @brief Checks that `drop` removes a package.
 */
void GivenTestSuite_LocalRepository_Drop() {
  const std::filesystem::path root = case_root("given");
  Expected<LocalRepository, std::string> repository =
      LocalRepository::load(root / "local.index", [](std::string_view) -> const Package* { return nullptr; });
  assert(repository && "the empty repository loads");

  repository->put(Package{"alpha", "d", PkgVersion::parse("1.0.0").value(), {}, {}}, true);
  assert(repository->search("alpha").has_value() && "the package is installed");

  repository->drop("alpha");
  assert(!repository->search("alpha") && "drop removes the package");
}

/**
 * @brief Checks that dropping an absent package is a no-op.
 */
void GivenTestSuite_LocalRepository_Drop_Absent() {
  const std::filesystem::path root = case_root("given");
  Expected<LocalRepository, std::string> repository =
      LocalRepository::load(root / "local.index", [](std::string_view) -> const Package* { return nullptr; });
  assert(repository && "the empty repository loads");

  repository->put(Package{"alpha", "d", PkgVersion::parse("1.0.0").value(), {}, {}}, true);
  assert(repository->search("alpha").has_value() && "the package is installed");

  repository->drop("beta");
  assert(repository->search("alpha").has_value() && "dropping an absent package is a no-op");
}

/**
 * @brief Checks that `fnv1a` matches the reference FNV-1a 64 vectors.
 */
void GivenTestSuite_LocalRepository_Fnv1a() {
  assert(LocalRepository::fnv1a("") == 14695981039346656037ULL && "the empty text hashes to the offset basis");
  assert(LocalRepository::fnv1a("a") == 12638187200555641996ULL && "a single byte mixes into the offset basis");
  assert(LocalRepository::fnv1a("foobar") == 9625390261332436968ULL && "the reference vector matches");
}
// NOLINTEND(misc-static-assert)
}  // namespace

TestSuite given_tests() {
  static const std::vector<TestCaseFnT> cases = {
      GivenTestSuite_Trie_Empty,
      GivenTestSuite_Trie_Insert_ReportsNew,
      GivenTestSuite_Trie_Insert_Stores,
      GivenTestSuite_Trie_Insert_DistinctBuckets,
      GivenTestSuite_Trie_Insert_SharedPrefix,
      GivenTestSuite_Trie_Insert_ReturnsIterator,
      GivenTestSuite_Trie_Find_Present,
      GivenTestSuite_Trie_Find_Absent,
      GivenTestSuite_Trie_Find_Const,
      GivenTestSuite_Trie_Erase,
      GivenTestSuite_Trie_Erase_Absent,
      GivenTestSuite_Trie_CopyConstructor,
      GivenTestSuite_Trie_CopyAssignment,
      GivenTestSuite_Trie_Iterate,
      GivenTestSuite_Trie_Iterate_Const,
      GivenTestSuite_Trie_Cbegin,
      GivenTestSuite_Trie_Clear_Empty,
      GivenTestSuite_Trie_Clear_Populated,
      GivenTestSuite_PkgVersion_Ordering,
      GivenTestSuite_PkgVersion_Equality,
      GivenTestSuite_PkgVersion_ToString,
      GivenTestSuite_PkgVersion_StreamOperator,
      GivenTestSuite_PackageManager_Install,
      GivenTestSuite_PackageManager_Install_Dependencies,
      GivenTestSuite_PackageManager_Install_AlreadyInstalled,
      GivenTestSuite_PackageManager_Install_NotFound,
      GivenTestSuite_PackageManager_Uninstall,
      GivenTestSuite_PackageManager_Upgrade_Version,
      GivenTestSuite_PackageManager_Upgrade_KeepsExplicit,
      GivenTestSuite_PackageManager_Prune,
      GivenTestSuite_PackageManager_Orphans,
      GivenTestSuite_LocalRepository_Put_Flag,
      GivenTestSuite_LocalRepository_Put_Dependents,
      GivenTestSuite_LocalRepository_Put_Replaces,
      GivenTestSuite_LocalRepository_Drop,
      GivenTestSuite_LocalRepository_Drop_Absent,
      GivenTestSuite_LocalRepository_Fnv1a,
  };
  return TestSuite{cases, 1, ALL_GIVEN_TESTS};
}
