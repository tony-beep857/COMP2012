#ifndef TRIE_H
#define TRIE_H

#include <array>
#include <optional>
#include <stack>
#include <type_traits>
#include <utility>
#include <variant>

template<typename T, typename Key, std::size_t N>
class TrieIterator;

/**
 * @brief A trie data structure for storing key-value pairs.
 *
 * @tparam T The value type.
 * @tparam Key The key type.
 * @tparam N The number of buckets.
 */
template<typename T, typename Key, std::size_t N>
class Trie {
  /**
   * @brief The type of a possibly-empty key-value pair stored in the trie.
   */
  using KvPairT = std::optional<std::pair<Key, T>>;

  /**
   * @brief The type of the array used to store sub-trie pointers.
   */
  using BucketsT = std::array<Trie*, N>;

 public:
  /**
   * @brief The type of keys in this trie.
   */
  using key_type = Key;

  /**
   * @brief The type of values in this trie.
   */
  using value_type = T;

  /**
   * @brief The size type.
   */
  using size_type = std::size_t;

  /**
   * @brief The difference type.
   */
  using difference_type = std::ptrdiff_t;

  /**
   * @brief The type of a mutable reference to values in this trie.
   */
  using reference = T&;

  /**
   * @brief The type of a const reference to values in this trie.
   */
  using const_reference = const T&;

  /**
   * @brief The type of a mutable pointer to values in this trie.
   */
  using pointer = T*;

  /**
   * @brief The type of a const pointer to values in this trie.
   */
  using const_pointer = const T*;

  /**
   * @brief The type of a mutable iterator over values in this trie.
   */
  using iterator = TrieIterator<T, Key, N>;

  /**
   * @brief The type of a const iterator over values in this trie.
   */
  using const_iterator = TrieIterator<const T, Key, N>;

  /**
   * @brief The function type of a key function, which extracts the key from a value.
   */
  using KeyFn = Key (*)(const T&);

  /**
   * @brief The function type of a bucket function, which determines the bucket index for a key at the given depth.
   *
   * It is undefined behavior if the returned bucket index is greater than or equal to `N`.
   */
  using BucketFn = std::size_t (*)(const Key&, std::size_t);

  /**
   * @brief Constructs a new instance with the given key and bucket functions.
   *
   * @param key_fn The key function, used to determine the associated key of a value.
   * @param bucket_fn The bucket function, used to determine the bucket index for a key.
   */
  Trie(KeyFn key_fn, BucketFn bucket_fn);

  /**
   * @brief Deletes every descendant node.
   */
  ~Trie();

  /**
   * @brief Deep-copies `other`: the copy owns its own nodes.
   *
   * @param other The trie to copy.
   */
  Trie(const Trie& other);

  /**
   * @brief Takes `other`'s nodes, leaving `other` empty.
   *
   * @param other The trie to move from.
   */
  Trie(Trie&& other) noexcept :
      key_fn(std::move(other.key_fn)),
      bucket_fn(std::move(other.bucket_fn)),
      depth(other.depth),
      kv_or_children(std::move(other.kv_or_children)) {
    other.kv_or_children = KvPairT{};
  }

  /**
   * @brief Replaces the contents with a deep copy of `other`.
   *
   * @param other The trie to copy.
   * @return This trie.
   */
  Trie& operator=(const Trie& other);

  /**
   * @brief Replaces the contents with `other`'s nodes, leaving `other` empty.
   *
   * @param other The trie to move from.
   * @return This trie.
   */
  Trie& operator=(Trie&& other) noexcept {
    if (this != &other) {
      clear();
      move_from(std::move(other));
    }
    return *this;
  }

  /**
   * @brief An iterator at the first stored value.
   *
   * @return The iterator, or `end()` when the trie is empty.
   */
  iterator begin() { return iterator{this}; }

  /**
   * @brief A const iterator at the first stored value.
   *
   * @return The iterator, or `end()` when the trie is empty.
   */
  const_iterator begin() const { return const_iterator{this}; }

  /**
   * @brief The past-the-end iterator.
   *
   * @return An iterator that compares equal to every exhausted iterator.
   */
  iterator end() { return iterator{nullptr}; }

  /**
   * @brief The past-the-end const iterator.
   *
   * @return An iterator that compares equal to every exhausted iterator.
   */
  const_iterator end() const { return const_iterator{nullptr}; }

  /**
   * @brief A const iterator at the first stored value.
   *
   * @return The iterator, or `cend()` when the trie is empty.
   */
  const_iterator cbegin() const { return begin(); }

  /**
   * @brief The past-the-end const iterator.
   *
   * @return An iterator that compares equal to every exhausted iterator.
   */
  const_iterator cend() const { return end(); }

  /**
   * @brief Whether nothing is stored.
   *
   * @return True when the trie holds no values.
   */
  [[nodiscard]] bool empty() const { return is_empty(); }

  /**
   * @brief Empties the trie.
   */
  void clear();

  /**
   * @brief Counts the stored values.
   *
   * @return How many values the trie holds.
   */
  [[nodiscard]] size_type size() const;

  /**
   * @brief Inserts `val` into the trie.
   *
   * @param val The value to insert.
   * @return An iterator at the stored value and whether inserted value is new.
   */
  std::pair<iterator, bool> insert(const T& val) {
    const Key key{key_fn(val)};
    const bool inserted{insert(key, val)};
    return std::make_pair(iterator{this, key}, inserted);
  }

  /**
   * @brief Looks a key up.
   *
   * @param key The key to find.
   * @return An iterator at the stored value, or `end()` when absent.
   */
  iterator find(const Key& key) { return iterator{this, key}; }

  /**
   * @brief Looks a key up through a const trie.
   *
   * @param key The key to find.
   * @return An iterator at the stored value, or `cend()` when absent.
   */
  const_iterator find(const Key& key) const { return const_iterator{this, key}; }

  /**
   * @brief Removes the value stored under `key`.
   *
   * @param key The key to remove.
   * @return The number of values removed, which is 0 or 1.
   */
  size_type erase(const Key& key) { return erase_from(key) ? 1 : 0; }

 private:
  /**
   * @brief Builds the node at `depth` with the given key and bucket functions.
   *
   * @param key_fn The key function.
   * @param bucket_fn The bucket function.
   * @param depth The depth of this node below the root.
   */
  Trie(KeyFn key_fn, BucketFn bucket_fn, std::size_t depth) :
      key_fn{key_fn}, bucket_fn{bucket_fn}, depth{depth}, kv_or_children{std::nullopt} {}

  /**
   * @brief The next level of trie that a value of this key routes to.
   *
   * @param key The key to route.
   * @return The slot, which the caller may fill.
   */
  Trie*& slot(const Key& key) { return std::get<BucketsT>(kv_or_children)[bucket_fn(key, depth)]; }

  /**
   * @brief Replaces this node's state with a deep copy of `other`.
   *
   * Copies the key and bucket functions, the depth and every sub-trie.
   *
   * @param other The node to copy.
   */
  void copy_from(const Trie& other);

  /**
   * @brief Takes `other`'s payload and leaves it an empty value node.
   *
   * @param other The node to move from.
   */
  void move_from(Trie&& other) {
    key_fn = other.key_fn;
    bucket_fn = other.bucket_fn;
    depth = other.depth;
    kv_or_children = std::move(other).kv_or_children;
    other.kv_or_children = KvPairT{};
  }

  /**
   * @brief Finds the value with the corresponding `key` from this node, and erasing it if found.
   *
   * If a value is erased, every sub-trie on the traversal path that is left holding neither a value nor any sub-trie
   * is deleted and its slot reset to `nullptr`. Only the root may be left empty.
   *
   * @param key The key to remove.
   * @return Whether a value was removed.
   */
  bool erase_from(const Key& key);

  /**
   * @brief Inserts `val` assuming that `key` is the computed key for `val`.
   *
   * @param key The key to insert under.
   * @param val The value to insert.
   * @return Whether the value was new.
   */
  bool insert(const Key& key, const T& val);

  /**
   * @brief Replaces the value stored in `kv_or_children` with an array of buckets, moving the value stored in this trie
   * to the next-level sub-trie.
   */
  void push_down();

  /**
   * @brief Returns the sub-trie slot for `key`, creating it if absent.
   *
   * @param key The key to route.
   * @return A reference to the pointer to the sub-trie.
   */
  Trie*& child(const Key& key);

  /**
   * @brief Whether this node stores a value.
   *
   * A value node is one that holds a key-value pair. Any other representations (an empty key-value pair or an array of
   * buckets) are not considered value nodes.
   *
   * @return True when this node holds a value.
   */
  [[nodiscard]] bool is_value() const;

  /**
   * @brief Whether this node holds neither a value nor any sub-trie.
   *
   * @return True when this node is empty.
   */
  [[nodiscard]] bool is_empty() const;

  friend class TrieIterator<T, Key, N>;
  friend class TrieIterator<std::add_const_t<T>, Key, N>;

  /**
   * @brief The function used to extract the key from a value.
   */
  KeyFn key_fn;

  /**
   * @brief The function used to map a key to a bucket index.
   */
  BucketFn bucket_fn;

  /**
   * @brief The depth of this node in the trie; `0` for the root node.
   */
  std::size_t depth;

  /**
   * @brief The value and its key if this node holds a value, or the array of sub-tries if this node contains multiple
   * values.
   */
  std::variant<KvPairT, BucketsT> kv_or_children;
};

/**
 * @brief Walks the values of a trie by pre-order traversal.
 *
 * Constness is carried by `T` itself, as `std::span<const T>` does: `TrieIterator<const V, Key, N>` is the const
 * variant, so `T&` and `T*` pick up the qualification with no further wiring and one body set compiles for both.
 *
 * @tparam T The value type. May be `const`-qualified if the iterator should yield `const`-qualified elements.
 * @tparam Key The key type.
 * @tparam N The number of buckets.
 */
template<typename T, typename Key, std::size_t N>
class TrieIterator {
  /**
   * @brief The trie type this iterator walks.
   */
  using TrieT = Trie<std::remove_const_t<T>, Key, N>;

  /**
   * @brief The type of a pointer to a node, const-qualified when the value type is const.
   */
  using NodePtr = std::conditional_t<std::is_const_v<T>, const TrieT, TrieT>*;

 public:
  /**
   * @brief The unqualified value type.
   */
  using value_type = std::remove_const_t<T>;

  /**
   * @brief The difference type.
   */
  using difference_type = std::ptrdiff_t;

  /**
   * @brief The iterator category.
   *
   * `std::forward_iterator_tag` indicates that the iterator can move forwards (`++`) but not backwards (`--`) nor
   * indexed (`+=`/`-=`).
   */
  using iterator_category = std::forward_iterator_tag;

  /**
   * @brief The type of a reference to a value.
   */
  using reference = T&;

  /**
   * @brief The type of a pointer to a value.
   */
  using pointer = T*;

  /**
   * @brief Whether two iterators are at the same value.
   *
   * @param other The iterator to compare against.
   * @return True when both sit on the same node or point to the past-the-end state.
   */
  bool operator==(const TrieIterator& other) const;
  bool operator!=(const TrieIterator& other) const { return !(*this == other); }

  /**
   * @brief Advances to the next value in pre-order.
   *
   * @return This iterator.
   */
  TrieIterator& operator++();

  /**
   * @brief Advances to the next value in pre-order, returning the old position.
   *
   * @return A copy of the iterator before the increment.
   */
  TrieIterator operator++(int) {
    TrieIterator tmp{*this};
    ++(*this);
    return tmp;
  }

  /**
   * @brief Reaches the current value.
   *
   * @return A reference to the value the iterator currently points to.
   */
  reference operator*() const;
  pointer operator->() const { return &(**this); }

 private:
  friend class Trie<std::remove_const_t<T>, Key, N>;

  /**
   * @brief Builds an iterator from the root of the trie, or at the past-the-end state if `root` is `nullptr` or the
   * trie is empty.
   *
   * @param root The root node of the trie.
   */
  explicit TrieIterator(NodePtr root);

  /**
   * @brief Builds an iterator beginning at the value node for `key` under the root node, or at the past-the-end state
   * when the trie holds no such key.
   *
   * @param root The root node of the trie.
   * @param key The key to begin traversal from.
   */
  explicit TrieIterator(NodePtr root, const Key& key);

  /**
   * @brief Finds the next non-empty node after `node` within the same parent trie.
   *
   * @param node The sub-trie after which the scan starts.
   * @return False when the parent has no later sibling that holds a value.
   */
  bool scan_siblings(std::add_const_t<NodePtr> node);

  /**
   * @brief Drops the entire root-to-current path, leaving the iterator at the past-the-end state.
   */
  void reset_path() { nodes = std::stack<NodePtr>{}; }

  /**
   * @brief Finds the next node with respect to the top-most node on the stack, pushing bucket nodes onto the
   * stack as needed.
   *
   * Note: This function assumes that the top-most node on the stack holds a value or is the non-empty node containing
   * sub-trie buckets.
   */
  void descend();

  /**
   * @brief A stack containing the nodes traversed from the root to the current node.
   *
   * The top-most node holds the current node, the bottom-most node holds the root node, and all bucket nodes hold
   * the path that leads to the current node.
   */
  std::stack<NodePtr> nodes;
};

#include "trie.tpp"

#endif  // TRIE_H
