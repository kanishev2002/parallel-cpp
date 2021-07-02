#pragma once

#include <mutex>
#include <ostream>
#include <string_view>
#include <thread>
#include <vector>

class HelloWorld {
 public:
  static constexpr std::string_view kHelloPrefix = "Hello, World! From thread ";

  HelloWorld(size_t n_threads) : n_threads(n_threads) {}

  void SayHello(std::ostream& os) {
    std::vector<std::thread> threads;
    for (size_t i = 0; i < n_threads; ++i) {
      threads.emplace_back([&] {
        m.lock();
        os << kHelloPrefix << std::this_thread::get_id() << '\n';
        m.unlock();
      });
    }
    for (size_t i = 0; i < n_threads; ++i) {
      threads[i].join();
    }
  }

 private:
  size_t n_threads;
  std::mutex m;
};
