#include <gtest/gtest.h>

#include "is_prime.h"
#include "timeout_guard.h"

TEST(IsPrime, One) {
  TimeoutGuard guard(10ms);
  ASSERT_FALSE(IsPrime(1));
}

TEST(IsPrime, Two) {
  TimeoutGuard guard(10ms);
  ASSERT_TRUE(IsPrime(2));
}

TEST(IsPrime, Nine) {
  TimeoutGuard guard(10ms);
  ASSERT_FALSE(IsPrime(9));
}

TEST(IsPrime, big_prime) {
  TimeoutGuard guard(100ms);
  ASSERT_TRUE(IsPrime(115249));
}

TEST(IsPrime, Even) {
  TimeoutGuard guard(500ms);
  for (size_t i = 4; i < 10000000; i += 2) {
    EXPECT_FALSE(IsPrime(i));
  }
}
