#pragma once

#include <atomic>
#include <thread>

class TicketLock {
 public:
  TicketLock() {}

  void Lock() {
    size_t ticket = new_ticket_.load();
    while (!new_ticket_.compare_exchange_weak(ticket, ticket + 1)) {
    }
    while (ticket != now_serving_.load()) {
      std::this_thread::yield();
    }
  }

  void Unlock() { ++now_serving_; }

 private:
  std::atomic<size_t> new_ticket_{0}, now_serving_{0};
};
