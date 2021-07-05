#pragma once

#include <atomic>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <thread>
#include <vector>

template <typename RandomAccessIterator, class T, class Func>
T parallel_reduce(RandomAccessIterator begin, RandomAccessIterator end,
                  const T& initial_value, Func func, size_t n_threads) {
  std::atomic<T> result{initial_value};
  const size_t size = static_cast<size_t>(end - begin);
  // const size_t items_per_thread = size / n_threads;
  std::vector<std::thread> threads;
  std::vector<T> results(n_threads, initial_value);
  for (size_t i = 0; i < n_threads; ++i) {
    threads.emplace_back([&, i] {
      T result = initial_value;
      auto from = begin + (i * size) / n_threads;
      auto to = begin + ((i + 1) * size) / n_threads;
      for (; from != to; ++from) {
        // result.store(func(result.load(), *from));
        result = func(result, *from);
      }
      results[i] = result;
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  return std::reduce(results.begin(), results.end(), initial_value, func);
}
