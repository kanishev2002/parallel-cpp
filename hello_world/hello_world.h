#pragma once

#include <mutex>
#include <ostream>
#include <string_view>
#include <thread>
#include <vector>

class HelloWorld {
 public:
  static constexpr std::string_view kHelloPrefix = "Hello, World! From thread ";

  HelloWorld(size_t n_threads) : threads_count_(n_threads) {}

  void SayHello(std::ostream& os) {
    std::vector<std::thread> threads;
    for (size_t i = 0; i < threads_count_; ++i) {
      threads.emplace_back([&] {
        std::unique_lock<std::mutex> lock(cout_mutex_);
        os << kHelloPrefix << std::this_thread::get_id() << '\n';
      });
    }
    for (auto& thread : threads) {
      thread.join();
    }
  }

 private:
  size_t threads_count_;
  std::mutex cout_mutex_;
};
