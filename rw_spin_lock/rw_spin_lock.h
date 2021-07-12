#pragma once

#include <atomic>
#include <thread>

class RWSpinLock {
 public:
  RWSpinLock() {}

  void LockRead() {
    while (true) {
      size_t current_state = lock_.load();
      if (current_state & 1) {
        std::this_thread::yield();
        continue;
      }
      if (lock_.compare_exchange_weak(current_state, current_state + 2)) {
        break;
      }
    }
  }

  void UnlockRead() {
    while (lock_ < 2) {
      std::this_thread::yield();
    }
    lock_ -= 2;
  }

  void LockWrite() {
    while (true) {
      size_t current_state = lock_.load();
      if (current_state & 1) {
        continue;
      }
      if (lock_.compare_exchange_weak(current_state, current_state + 1)) {
        break;
      }
    }
    while (lock_ != 1) {
      std::this_thread::yield();
    }
  }

  void UnlockWrite() {
    while ((lock_ & 1) == 0) {
      std::this_thread::yield();
    }
    --lock_;
  }

 private:
  std::atomic<size_t> lock_ = 0;
};
