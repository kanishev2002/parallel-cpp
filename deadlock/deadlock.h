#pragma once

#include <mutex>
#include <thread>

class Deadlock {
 public:
  Deadlock() {}

  void ThreadOne() {
    // Your
    m1.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    m2.lock();
    m2.unlock();
    m1.unlock();
  }

  void ThreadTwo() {
    // Your
    m2.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    m1.lock();
    m1.unlock();
    m2.unlock();
  }

 private:
  std::mutex m1, m2;
  // Your
};
