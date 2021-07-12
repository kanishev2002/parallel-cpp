#pragma once

#include <mutex>
#include <shared_mutex>
#include <vector>

template <typename T>
class ThreadSafeVector {
 public:
  ThreadSafeVector() {}

  T operator[](size_t index) const {
    std::shared_lock lock(s_read_mutex_);
    return vector_[index];
  }

  size_t Size() const {
    std::shared_lock lock(s_push_mutex_);
    return vector_.size();
  }

  void PushBack(const T& value) {
    std::unique_lock lock(s_push_mutex_);
    if (vector_.capacity() == vector_.size()) {
      std::unique_lock lock(s_read_mutex_);
      vector_.push_back(value);
    } else {
      vector_.push_back(value);
    }
  }

 private:
  std::vector<T> vector_;
  mutable std::shared_mutex s_read_mutex_, s_push_mutex_;
};
