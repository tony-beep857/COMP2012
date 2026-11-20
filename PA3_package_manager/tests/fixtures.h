#ifndef FIXTURES_H
#define FIXTURES_H

#include <filesystem>
#include <set>
#include <string>
#include <vector>

#include "trie.h"

template<typename T, typename Key, std::size_t N>
class TrieSpecialization {
 public:
  using underlying_type = Trie<T, Key, N>;
  using iterator = typename underlying_type::iterator;
  using const_iterator = typename underlying_type::const_iterator;

  TrieSpecialization(typename underlying_type::KeyFn key_fn, typename underlying_type::BucketFn bucket_fn) :
      trie{key_fn, bucket_fn} {}

  static constexpr std::size_t SPAN = N;

  /**
   * @brief Collects the values the trie's iterator walks.
   *
   * @param trie The trie to walk.
   * @return The values, in iteration order if the container is sequenced.
   */
  template<typename Collection = std::vector<T>>
  Collection collect() const {
    Collection values;
    for (const T& value : trie) {
      values.emplace_back(value);
    }
    return values;
  }

  underlying_type& operator*() noexcept { return trie; }
  const underlying_type& operator*() const noexcept { return trie; }
  underlying_type* operator->() noexcept { return &trie; }
  const underlying_type* operator->() const noexcept { return &trie; }

 private:
  underlying_type trie;
};

/**
 * @brief Wipes and recreates a directory in the working directory, unique per suite and case.
 *
 * The directory is named `pa3_test_<suite>_<position>`, where `<position>` is the case's 1-based position in its
 * suite's registry, taken from `current_case_position`. A case therefore starts from an empty tree however an earlier
 * run left it, and adding or moving a case renames its fixture without touching the case body.
 *
 * @param suite The suite name.
 * @return The recreated directory.
 */
std::filesystem::path case_root(const std::string& suite);

/**
 * @brief Writes `text` to `path`, asserting when the file cannot be written.
 *
 * @param path The file to write.
 * @param text The contents to write.
 */
void write_text(const std::filesystem::path& path, const std::string& text);

/**
 * @brief Writes a package file named after the package.
 *
 * @param dir The directory to write into.
 * @param name The package name, and the file name.
 * @param version The package version.
 * @param depends Space-separated dependency names; omitted when empty.
 * @param conflicts Space-separated conflict names; omitted when empty.
 */
void write_package(const std::filesystem::path& dir,
                   const std::string& name,
                   const std::string& version,
                   const std::string& depends = "",
                   const std::string& conflicts = "");

/**
 * @brief Writes a remote index listing `names`, one per line.
 *
 * @param dir The directory holding the index and the package files.
 * @param names The package names to list.
 * @return The index path, so the caller can pass it to `open`.
 */
std::filesystem::path write_remote(const std::filesystem::path& dir, const std::vector<std::string>& names);

/**
 * @brief Writes a local index, one line per entry.
 *
 * @param path The local index file.
 * @param lines The index lines, already in `<pkgname> <pkgver> [explicit]` form.
 */
void write_local_index(const std::filesystem::path& path, const std::vector<std::string>& lines);

/**
 * @brief Writes the shared fixture repository.
 *
 * The packages create both a dependency chain and a conflict pair:
 * - `numcpp`, `octplotlib`, `blanket`: leaves;
 * - `tensorstream`: depends on `numcpp` and `octplotlib`, conflicts with `cpptorch`;
 * - `cpptorch`: depends on `numcpp`, conflicts with `tensorstream`;
 * - `sarek`: depends on `tensorstream`, conflicts with `scikit-img`;
 * - `scikit-img`: a leaf.
 *
 * @param dir The directory holding the index and the package files.
 * @return The index path, so the caller can pass it to `open`.
 */
std::filesystem::path write_standard_remote(const std::filesystem::path& dir);

/**
 * @brief Collects `values` into a set, so iteration order does not matter.
 *
 * @param values The values to collect.
 * @return The distinct values, sorted.
 */
std::set<int> as_set(const std::vector<int>& values);

#endif  // FIXTURES_H
