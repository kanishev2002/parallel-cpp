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
  ThreadSafeQueue() {}

  void Push(const T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(value);
    cv_.notify_all();
  }

  T Pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [&] { return !queue_.empty(); });
    auto value = queue_.front();
    queue_.pop();
    return value;
  }

  std::optional<T> TryPop() {
    std::unique_lock<std::mutex> lock(mutex_);
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
};
