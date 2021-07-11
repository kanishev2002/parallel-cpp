#pragma once

#include <atomic>
#include <thread>

class MCSLock {
 public:
  MCSLock() {}
  struct Node {
    Node() : next(nullptr), locked(false) {}
    Node* next;
    bool locked;
  };

  void Lock() {
    Node* prev = last_.exchange(&node);
    if (prev != nullptr) {
      node.locked = true;
      prev->next = &node;
      while (node.locked)
        ;
    }
  }

  void Unlock() {
    const auto next = node.next;
    if (next == nullptr) {
      Node* expected = &node;
      if (last_.compare_exchange_weak(expected, nullptr)) {
        return;
      }
    }
    while (next == nullptr)
      ;
    next->locked = false;
  }

 private:
  thread_local static inline Node node;
  std::atomic<Node*> last_;
};
