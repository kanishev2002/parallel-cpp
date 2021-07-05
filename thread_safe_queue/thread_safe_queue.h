#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

template <typename T>
class ThreadSafeQueue {
 public:
  ThreadSafeQueue() : is_pushing_(false) {}

  void Push(const T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    is_pushing_.store(true);
    queue_.push(value);
    is_pushing_.store(false);
    cv_.notify_all();
  }

  T Pop() {
    for (;;) {
      std::unique_lock<std::mutex> lock(mutex_);
      cv_.wait(lock, [&] { return !queue_.empty() || !is_pushing_; });
      if (!is_pushing_ && !queue_.empty()) {
        auto value = queue_.front();
        queue_.pop();
        return value;
      }
    }
  }

  std::optional<T> TryPop() {
    // Your code
    while (is_pushing_) {
      std::this_thread::yield();
    }
    if (queue_.empty()) {
      return std::nullopt;
    }
    auto value = queue_.front();
    queue_.pop();
    return value;
  }

 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cv_;
  std::atomic<bool> is_pushing_;
};
