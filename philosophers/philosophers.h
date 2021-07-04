#pragma once

#include <mutex>

class Fork {
 public:
  Fork(size_t id) : id_(id) {}

  size_t Id() const { return id_; }

  void Get() { mutex_.lock(); }

  bool TryGet() { return mutex_.try_lock(); }

  void Put() { mutex_.unlock(); }

 private:
  size_t id_;
  std::mutex mutex_;
};

class Philosopher {
 public:
  Philosopher(size_t id, Fork* left_fork, Fork* right_fork)
      : philosopher_id_(id), left_fork_(left_fork), right_fork_(right_fork) {}

  size_t Id() const { return philosopher_id_; }

  void Eat() {
    if (left_fork_->Id() < right_fork_->Id()) {
      left_fork_->Get();
      right_fork_->Get();
    } else {
      right_fork_->Get();
      left_fork_->Get();
    }
  }

  void Think() {
    left_fork_->Put();
    right_fork_->Put();
  }

 private:
  size_t philosopher_id_;
  Fork *left_fork_, *right_fork_;
};
