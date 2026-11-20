#ifndef TRIE_TPP
#define TRIE_TPP

#ifndef TRIE_H
#include "trie.h"
#endif

#include <algorithm>
#include <cassert>
#include <utility>

template<typename T, typename Key, std::size_t N>
Trie<T, Key, N>::Trie(Trie::KeyFn key_fn, Trie::BucketFn bucket_fn) : Trie{key_fn, bucket_fn, 0} {
  // TODO
}

template<typename T, typename Key, std::size_t N>
Trie<T, Key, N>::~Trie() {
  // TODO  
  clear();
}

template<typename T, typename Key, std::size_t N>
Trie<T, Key, N>::Trie(const Trie& other) :   Trie{other.key_fn, other.bucket_fn, other.depth} {
  // TODO
  copy_from(other);
}

template<typename T, typename Key, std::size_t N>
Trie<T, Key, N>& Trie<T, Key, N>::operator=(const Trie& other) {
  // TODO
  if (this != &other) {
    copy_from(other);
  }
  return *this;
}

template<typename T, typename Key, std::size_t N>
void Trie<T, Key, N>::clear() {
  // TODO
  if (BucketsT* buckets = std::get_if<BucketsT>(&kv_or_children)) {
    for (Trie* sub_trie : *buckets) {
      delete sub_trie;
    }
  }
  kv_or_children = KvPairT{};
}

template<typename T, typename Key, std::size_t N>
typename Trie<T, Key, N>::size_type Trie<T, Key, N>::size() const {
  // TODO
  if (const KvPairT* kv = std::get_if<KvPairT>(&kv_or_children)) {
    return kv->has_value() ? 1 : 0;
  }

  size_type total = 0;
  const BucketsT& buckets = std::get<BucketsT>(kv_or_children);
  for (const Trie* sub_trie : buckets) {
    if (sub_trie != nullptr) {
      total += sub_trie->size();
    }
  }
  return total;
}

template<typename T, typename Key, std::size_t N>
void Trie<T, Key, N>::copy_from(const Trie& other) {
  // TODO(Optional)
  // does not check (this == other)
  clear();
  key_fn = other.key_fn;
  bucket_fn = other.bucket_fn;
  depth = other.depth;

  if(const KvPairT* other_kv = std::get_if<KvPairT>(&other.kv_or_children)){
    kv_or_children = *other_kv;
    return;
  }

  kv_or_children = BucketsT{};
  BucketsT& buckets = std::get<BucketsT>(kv_or_children);
  const BucketsT& other_buckets = std::get<BucketsT>(other.kv_or_children);
  for (std::size_t i = 0; i < N; ++i){
    if (other_buckets[i] != nullptr){
      buckets[i] = new Trie{*other_buckets[i]};
    }
  }
}

template<typename T, typename Key, std::size_t N>
bool Trie<T, Key, N>::erase_from(const Key& key) {
  // TODO
  if (KvPairT* kv = std::get_if<KvPairT>(&kv_or_children)) {
    if (kv->has_value() && (*kv)->first == key) {
      kv->reset();
      return true;
    }
    return false;
  }

  Trie*& sub_trie = slot(key);
  if (sub_trie == nullptr) {
    return false;
  }

  const bool removed = sub_trie->erase_from(key);
  if (removed && sub_trie->is_empty()) {
    delete sub_trie;
    sub_trie = nullptr;
    if (depth == 0 && is_empty()) {
      kv_or_children = KvPairT{};
    }
  }
  return removed;
}

template<typename T, typename Key, std::size_t N>
bool Trie<T, Key, N>::insert(const Key& key, const T& val) {
  // TODO
  if (is_empty()) {
    kv_or_children = KvPairT{std::pair<Key, T>{key, val}};
    return true;
  }

  if (const KvPairT* kv = std::get_if<KvPairT>(&kv_or_children)) {
    if ((*kv)->first == key) {
      return false;
    }
    push_down();
  }

  return child(key)->insert(key, val);
}

template<typename T, typename Key, std::size_t N>
void Trie<T, Key, N>::push_down() {
// TODO(Optional)
  // did not check the node has value or not
  KvPairT kv{std::move(std::get<KvPairT>(kv_or_children))};
  kv_or_children = BucketsT{};
  Trie* next = child(kv->first);
  next->kv_or_children = std::move(kv);
}

template<typename T, typename Key, std::size_t N>
Trie<T, Key, N>*& Trie<T, Key, N>::child(const Key& key) {
  // TODO(Optional)
  Trie*& target = slot(key);
  if (target == nullptr){
    target = new Trie{key_fn, bucket_fn, depth + 1};
  }

  return target;
}

template<typename T, typename Key, std::size_t N>
bool Trie<T, Key, N>::is_value() const {
  // TODO(Optional)
  const KvPairT* kv = std::get_if<KvPairT>(&kv_or_children);
  if (kv != nullptr && kv->has_value())
    return true;
  return false;
}

template<typename T, typename Key, std::size_t N>
bool Trie<T, Key, N>::is_empty() const {
  // TODO
  if (const KvPairT* kv = std::get_if<KvPairT>(&kv_or_children))
    return !kv->has_value();
  const BucketsT& buckets = std::get<BucketsT>(kv_or_children);
    for (const Trie* child : buckets){
      if (child != nullptr)
        return false;
    }
  return true;
}

template<typename T, typename Key, std::size_t N>
TrieIterator<T, Key, N>::TrieIterator(TrieIterator::NodePtr root) {
  // TODO
  if (root == nullptr || root->is_empty()) {
    return;
  }
  nodes.push(root);
  descend();
}

template<typename T, typename Key, std::size_t N>
TrieIterator<T, Key, N>::TrieIterator(NodePtr root, const Key& key) {
  // TODO
  NodePtr curr = root;
  while (curr != nullptr) {
    nodes.push(curr);

    if (const typename TrieT::KvPairT* kv =
            std::get_if<typename TrieT::KvPairT>(&curr->kv_or_children)) {
      if (kv->has_value() && (*kv)->first == key) {
        return;
      }
      reset_path();
      return;
    }

    const typename TrieT::BucketsT& buckets =
        std::get<typename TrieT::BucketsT>(curr->kv_or_children);
    curr = buckets[curr->bucket_fn(key, curr->depth)];
  }

  reset_path();
}

template<typename T, typename Key, std::size_t N>
bool TrieIterator<T, Key, N>::operator==(const TrieIterator& other) const {
  // TODO
  if (nodes.empty() || other.nodes.empty()) {
    return nodes.empty() && other.nodes.empty();
  }
  return nodes.top() == other.nodes.top();
}

template<typename T, typename Key, std::size_t N>
TrieIterator<T, Key, N>& TrieIterator<T, Key, N>::operator++() {
  // TODO
  while (!nodes.empty()) {
    NodePtr current = nodes.top();
    nodes.pop();

    if (scan_siblings(current)) {
      descend();
      return *this;
    }
  }
  return *this;
}

template<typename T, typename Key, std::size_t N>
typename TrieIterator<T, Key, N>::reference TrieIterator<T, Key, N>::operator*() const {
  // TODO
  return std::get<typename TrieT::KvPairT>(nodes.top()->kv_or_children)->second;
}

template<typename T, typename Key, std::size_t N>
bool TrieIterator<T, Key, N>::scan_siblings(std::add_const_t<NodePtr> node) {
// TODO(Optional)
  if (nodes.empty()) {
    return false;
  }

  const typename TrieT::BucketsT& buckets =
      std::get<typename TrieT::BucketsT>(nodes.top()->kv_or_children);

  std::size_t i = 0;
  while (i < N && buckets[i] != node) {
    ++i;
  }

  for (++i; i < N; ++i) {
    if (buckets[i] != nullptr) {
      nodes.push(buckets[i]);
      return true;
    }
  }
  return false;
}

template<typename T, typename Key, std::size_t N>
void TrieIterator<T, Key, N>::descend() {
// TODO(Optional)
  while (!nodes.empty() && !nodes.top()->is_value()) {
    const typename TrieT::BucketsT* buckets =
        std::get_if<typename TrieT::BucketsT>(&nodes.top()->kv_or_children);
    if (buckets == nullptr) {
      reset_path();
      return;
    }

    NodePtr next = nullptr;
    for (NodePtr sub_trie : *buckets) {
      if (sub_trie != nullptr) {
        next = sub_trie;
        break;
      }
    }

    if (next == nullptr) {
      reset_path();
      return;
    }
    nodes.push(next);
  }
}

#endif  // TRIE_TPP
