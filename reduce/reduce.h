#pragma once

#include <atomic>
#include <cstddef>
#include <iterator>
#include <thread>
#include <vector>

template <typename RandomAccessIterator, class T, class Func>
T parallel_reduce(RandomAccessIterator begin, RandomAccessIterator end,
                  const T& initial_value, Func func, size_t n_threads) {
  std::atomic<T> result{initial_value};
  const size_t size = static_cast<size_t>(end - begin);
  const size_t items_per_thread = size / n_threads;
  std::vector<std::thread> threads;
  for (size_t i = 0; i < n_threads - 1; i += items_per_thread) {
    threads.emplace_back([&, i] {
      auto from = std::next(begin, i);
      auto to = std::next(from, items_per_thread);
      for (; from != to; ++from) {
        // result.store(func(result.load(), *from));
        T tmp(result.load());
        while (!result.compare_exchange_weak(tmp, func(*from, result))) {
          std::this_thread::yield();
        }
      }
    });
  }
  threads.emplace_back([&] {
    for (auto from = std::next(begin, (n_threads - 1) * items_per_thread);
         from != end; ++from) {
      T tmp = result.load();
      // result = func(result, *from);
      while (!result.compare_exchange_weak(tmp, func(*from, result))) {
        std::this_thread::yield();
      }
    }
  });
  for (auto& thread : threads) {
    thread.join();
  }
  return result.load();
}
