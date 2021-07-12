#pragma once

#include <atomic>
#include <thread>

class MCSLock {
 public:
  MCSLock() {}
  struct Node {
    Node() : next(nullptr), locked(false) {}
    std::atomic<Node*> next;
    std::atomic<bool> locked;
  };

  void Lock() {
    Node* prev = last_.exchange(&node);
    if (prev != nullptr) {
      node.locked.store(true);
      prev->next.store(&node);
      while (node.locked.load()) {
        std::this_thread::yield();
      }
    }
  }

  void Unlock() {
    if (node.next.load() == nullptr) {
      Node* expected = &node;
      if (last_.compare_exchange_weak(expected, nullptr)) {
        return;
      }
      while (node.next.load() == nullptr) {
      }
    }

    node.next.load()->locked = false;
    node.next.store(nullptr);
  }

 private:
  thread_local static Node node;
  std::atomic<Node*> last_ = nullptr;
};
