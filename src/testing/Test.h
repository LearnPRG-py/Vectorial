#include <cassert>
#include <iostream>
#include <thread>

#include "../Vectorial.h"

#ifndef TEST_H
#define TEST_H

// Helpers
template <typename T> bool ExpectEQ(T a, T b) {
  if (a == b) {
    std::cout << "PASS! " << a << " is equal to " << b
              << ". Expectation satisfied. \n";
    return true;
  } else {
    std::cout << "FAIL! Expected equality of: " << a << " and " << b << "\n";
    return false;
  }
}

template <typename T> bool ExpectNEQ(T a, T b) {
  if (a != b) {
    std::cout << "PASS! " << a << " is not equal to " << b
              << ". Expectation satisfied. \n";
    return true;
  } else {
    std::cout << "FAIL! Expected inequality of: " << a << " and " << b << "\n";
    return false;
  }
}

template <typename T> void AssertEQ(T a, T b) { assert(a == b); }

bool SimpleVectorTest() {
  constexpr int kTestCount = 9;
  bool should_pass = true;
  std::cout << "Running suite SimpleVectorTest: " << kTestCount
            << " Testcases available.\n";

  // Test 1: No crash on init and push_back
  std::cout << "[1/" << kTestCount << "] - No crash on init.\n";
  {
    Vector<int, 3> buffer;
    int list[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
      buffer.push_back(list[i]);
    }
    std::cout << "PASS! No crash. Test Passes.\n";
  }

  // Test 2: Simple buffer lookup - most recent value
  std::cout << "[2/" << kTestCount
            << "] - Simple buffer lookup (most recent)\n";
  {
    Vector<int, 3> buffer;
    buffer.push_back(5);
    should_pass = ExpectEQ(5, buffer[0]) && should_pass;
  }

  // Test 3: Check all entries after push of 5 values (cycling)
  std::cout << "[3/" << kTestCount << "] - Buffer contents after cycling\n";
  {
    Vector<int, 3> buffer;
    int list[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
      buffer.push_back(list[i]);
    }
    should_pass = ExpectEQ(5, buffer[0]) && should_pass;
    should_pass = ExpectEQ(4, buffer[1]) && should_pass;
    should_pass = ExpectEQ(3, buffer[2]) && should_pass;
  }

  // Test 4: size() returns correct count
  std::cout << "[4/" << kTestCount
            << "] - size() returns correct number of entries\n";
  {
    Vector<int, 3> buffer;
    should_pass = ExpectEQ(0u, buffer.size()) && should_pass;
    buffer.push_back(10);
    should_pass = ExpectEQ(1u, buffer.size()) && should_pass;
    buffer.push_back(20);
    buffer.push_back(30);
    should_pass = ExpectEQ(3u, buffer.size()) && should_pass;
    buffer.push_back(40);
    should_pass = ExpectEQ(3u, buffer.size()) && should_pass;
  }

  // Test 5: reset() clears buffer
  std::cout << "[5/" << kTestCount << "] - reset() clears buffer\n";
  {
    Vector<int, 3> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.reset();
    should_pass = ExpectEQ(0u, buffer.size()) && should_pass;
    buffer.push_back(99);
    should_pass = ExpectEQ(99, buffer[0]) && should_pass;
  }

  // Test 6: ignore mode prevents overflow
  std::cout << "[6/" << kTestCount
            << "] - ignore mode does not add when full\n";
  {
    Vector<int, 3> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.push_back(4, appendType::ignore);
    should_pass = ExpectEQ(3u, buffer.size()) && should_pass;
    should_pass = ExpectEQ(3, buffer[0]) && should_pass;
    should_pass = ExpectEQ(2, buffer[1]) && should_pass;
    should_pass = ExpectEQ(1, buffer[2]) && should_pass;
  }

  // Test 7: get_value returns success for valid index
  std::cout << "[7/" << kTestCount << "] - get_value success for valid index\n";
  {
    Vector<int, 3> buffer;
    buffer.push_back(42);
    auto result = buffer.get_value(0);
    should_pass = ExpectEQ(true, result.success) && should_pass;
    should_pass = ExpectEQ(42, result.value) && should_pass;
  }

  // Test 8: get_value returns failure for invalid index
  std::cout << "[8/" << kTestCount
            << "] - get_value fails for out-of-bounds index\n";
  {
    Vector<int, 3> buffer;
    buffer.push_back(10);
    auto result = buffer.get_value(5);
    should_pass = ExpectEQ(false, result.success) && should_pass;
  }

  // Test 9: Cycling overwrites oldest
  std::cout << "[9/" << kTestCount << "] - Cycling overwrites oldest value\n";
  {
    Vector<int, 3> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.push_back(4);
    should_pass = ExpectEQ(4, buffer[0]) && should_pass;
    should_pass = ExpectEQ(3, buffer[1]) && should_pass;
    should_pass = ExpectEQ(2, buffer[2]) && should_pass;
  }

  if (should_pass) {
    std::cout << "SimpleVectorTest: All " << kTestCount << " tests passed!\n";
  } else {
    std::cout << "SimpleVectorTest: Some tests failed.\n";
  }
  return should_pass;
}

bool TimedVectorAsVectorTest() {
  constexpr int kTestCount = 9;
  bool should_pass = true;
  std::cout << "Running suite TimedVectorAsVectorTest: " << kTestCount
            << " Testcases available.\n";

  // Test 1: No crash on init and push_back
  std::cout << "[1/" << kTestCount << "] - No crash on init.\n";
  {
    TimedVector<int, 3> buffer;
    int list[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
      buffer.push_back(list[i]);
    }
    std::cout << "PASS! No crash. Test Passes.\n";
  }

  // Test 2: Simple buffer lookup - most recent value
  std::cout << "[2/" << kTestCount
            << "] - Simple buffer lookup (most recent)\n";
  {
    TimedVector<int, 3> buffer;
    buffer.push_back(5);
    should_pass = ExpectEQ(5, buffer[0].value) && should_pass;
  }

  // Test 3: Check all entries after push of 5 values (cycling)
  std::cout << "[3/" << kTestCount << "] - Buffer contents after cycling\n";
  {
    TimedVector<int, 3> buffer;
    int list[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
      buffer.push_back(list[i]);
    }
    should_pass = ExpectEQ(5, buffer[0].value) && should_pass;
    should_pass = ExpectEQ(4, buffer[1].value) && should_pass;
    should_pass = ExpectEQ(3, buffer[2].value) && should_pass;
  }

  // Test 4: size() returns correct count
  std::cout << "[4/" << kTestCount
            << "] - size() returns correct number of entries\n";
  {
    TimedVector<int, 3> buffer;
    should_pass = ExpectEQ(0u, buffer.size()) && should_pass;
    buffer.push_back(10);
    should_pass = ExpectEQ(1u, buffer.size()) && should_pass;
    buffer.push_back(20);
    buffer.push_back(30);
    should_pass = ExpectEQ(3u, buffer.size()) && should_pass;
    buffer.push_back(40);
    should_pass = ExpectEQ(3u, buffer.size()) && should_pass;
  }

  // Test 5: reset() clears buffer
  std::cout << "[5/" << kTestCount << "] - reset() clears buffer\n";
  {
    TimedVector<int, 3> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.reset();
    should_pass = ExpectEQ(0u, buffer.size()) && should_pass;
    buffer.push_back(99);
    should_pass = ExpectEQ(99, buffer[0].value) && should_pass;
  }

  // Test 6: ignore mode prevents overflow
  std::cout << "[6/" << kTestCount
            << "] - ignore mode does not add when full\n";
  {
    TimedVector<int, 3> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.push_back(4, appendType::ignore);
    should_pass = ExpectEQ(3u, buffer.size()) && should_pass;
    should_pass = ExpectEQ(3, buffer[0].value) && should_pass;
    should_pass = ExpectEQ(2, buffer[1].value) && should_pass;
    should_pass = ExpectEQ(1, buffer[2].value) && should_pass;
  }

  // Test 7: get_value returns success for valid index
  std::cout << "[7/" << kTestCount << "] - get_value success for valid index\n";
  {
    TimedVector<int, 3> buffer;
    buffer.push_back(42);
    auto result = buffer.get_value(0);
    should_pass = ExpectEQ(true, result.success) && should_pass;
    should_pass = ExpectEQ(42, result.value.value) && should_pass;
  }

  // Test 8: get_value returns failure for invalid index
  std::cout << "[8/" << kTestCount
            << "] - get_value fails for out-of-bounds index\n";
  {
    TimedVector<int, 3> buffer;
    buffer.push_back(10);
    auto result = buffer.get_value(5);
    should_pass = ExpectEQ(false, result.success) && should_pass;
  }

  // Test 9: Cycling overwrites oldest
  std::cout << "[9/" << kTestCount << "] - Cycling overwrites oldest value\n";
  {
    TimedVector<int, 3> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.push_back(4);
    should_pass = ExpectEQ(4, buffer[0].value) && should_pass;
    should_pass = ExpectEQ(3, buffer[1].value) && should_pass;
    should_pass = ExpectEQ(2, buffer[2].value) && should_pass;
  }

  if (should_pass) {
    std::cout << "TimedVectorAsVectorTest: All " << kTestCount << " tests passed!\n";
  } else {
    std::cout << "TimedVectorAsVectorTest: Some tests failed.\n";
  }
  return should_pass;
}

bool TimedVectorTimeTest() {
  constexpr int kTestCount = 4;
  bool should_pass = true;
  std::cout << "Running suite TimedVectorTimeTest: " << kTestCount
            << " Testcases available.\n";

  // Test 1: Time field exists (not zero)
  std::cout << "[1/" << kTestCount << "] - Time field is populated\n";
  {
    TimedVector<int, 3> buffer;
    #ifdef CI
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    #else
    delay(2);
    #endif
    buffer.push_back(100);
    should_pass = ExpectNEQ(0u, buffer[0].time) && should_pass;
  }

  // Test 2: Multiple entries have times
  std::cout << "[2/" << kTestCount << "] - All entries have timestamps\n";
  {
    TimedVector<int, 3> buffer;
    #ifdef CI
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    #else
    delay(2);
    #endif
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    should_pass = ExpectNEQ(0u, buffer[0].time) && should_pass;
    should_pass = ExpectNEQ(0u, buffer[1].time) && should_pass;
    should_pass = ExpectNEQ(0u, buffer[2].time) && should_pass;
  }

  // Test 3: Manual DataPoint push works
  std::cout << "[3/" << kTestCount << "] - Manual DataPoint with custom time\n";
  {
    TimedVector<int, 3> buffer;
    TimedVector<int, 3>::DataPoint dp;
    dp.value = 42;
    dp.time = 12345;
    buffer.push_back(dp);
    should_pass = ExpectEQ(42, buffer[0].value) && should_pass;
    should_pass = ExpectEQ(12345u, buffer[0].time) && should_pass;
  }

  // Test 4: get_value returns DataPoint with time
  std::cout << "[4/" << kTestCount << "] - get_value returns timestamped data\n";
  {
    TimedVector<int, 3> buffer;
    #ifdef CI
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    #else
    delay(2);
    #endif
    buffer.push_back(999);
    auto result = buffer.get_value(0);
    should_pass = ExpectEQ(true, result.success) && should_pass;
    should_pass = ExpectEQ(999, result.value.value) && should_pass;
    should_pass = ExpectNEQ(0u, result.value.time) && should_pass;
  }

  if (should_pass) {
    std::cout << "TimedVectorTimeTest: All " << kTestCount << " tests passed!\n";
  } else {
    std::cout << "TimedVectorTimeTest: Some tests failed.\n";
  }
  return should_pass;
}

void TestRunner() {
  std::cout << "============================================================\n";
  bool success = SimpleVectorTest();
  std::cout << "============================================================\n";
  success = TimedVectorAsVectorTest() && success;
  std::cout << "============================================================\n";
  success = TimedVectorTimeTest() && success;
  std::cout << "============================================================\n";
  assert(success);
}

#endif