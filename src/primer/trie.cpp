#include "primer/trie.h"
#include <string_view>
#include "common/exception.h"

#include <iostream>

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.
  auto curTrieNode = root_;
  for(auto c : key){
    auto iter = curTrieNode->children_.find(c);
    if(iter == curTrieNode->children_.end()){
      return nullptr;
    }
    curTrieNode = iter->second;
  }

  const TrieNodeWithValue<T>* res = dynamic_cast<const TrieNodeWithValue<T>*>(curTrieNode.get());
  if(res == nullptr){
    return nullptr;
  }
  return res->value_.get();
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {  
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // throw NotImplementedException("Trie::Put is not implemented.");

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
  std::shared_ptr<TrieNode> resTrieRoot;
  if(root_ == nullptr){
    resTrieRoot = std::make_shared<TrieNode>();
  }
  else if(key.size() == 0){
    resTrieRoot = std::make_shared<TrieNodeWithValue<T>>(std::move(root_->children_), std::make_shared<T>(std::move(value)));
  }
  else{
    resTrieRoot = std::shared_ptr<TrieNode>(std::move(root_->Clone()));
  }
  std::shared_ptr<TrieNode> curTrieNode = resTrieRoot;
  for(size_t i = 0; i < key.size(); i++){
    auto c = key.at(i);
    auto iter = curTrieNode->children_.find(c);
    if(i == key.size() - 1){
      std::shared_ptr<TrieNodeWithValue<T>> newValueNode;
      if(iter == curTrieNode->children_.end()){
        newValueNode = std::make_shared<TrieNodeWithValue<T>>(std::make_shared<T>(std::move(value)));
        curTrieNode->children_.insert(std::pair<char, std::shared_ptr<TrieNode>>(c, std::move(newValueNode)));
      }
      else{
        newValueNode = std::make_shared<TrieNodeWithValue<T>>(std::move(iter->second->children_), std::make_shared<T>(std::move(value)));
        curTrieNode->children_.erase(c);
        curTrieNode->children_.insert(std::pair<char, std::shared_ptr<TrieNode>>(c, std::move(newValueNode)));
      }
    }
    else{
      std::shared_ptr<TrieNode> newChildNode;
      if(iter == curTrieNode->children_.end()){
        newChildNode = std::make_shared<TrieNode>();
      }
      else{
        newChildNode = std::shared_ptr<TrieNode>(std::move(iter->second->Clone()));
        curTrieNode->children_.erase(c);
      }
      curTrieNode->children_.insert(std::pair<char, std::shared_ptr<TrieNode>>(c, newChildNode));
      curTrieNode = std::move(newChildNode);
    }
  }
  return Trie(resTrieRoot);
}

auto Trie::Remove(std::string_view key) const -> Trie {
  throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
