#ifndef VECTORIAL_H
#define VECTORIAL_H

#include <cstdint>

#ifndef CI
#include <Arduino.h>
#else
#include <chrono>
static auto start_time = std::chrono::steady_clock::now();

inline uint32_t millis() {
  auto now = std::chrono::steady_clock::now();
  auto elapsed = now - start_time;
  return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
}
#endif

enum appendType { cycle, ignore };
inline uint32_t vec_min(uint32_t a, uint32_t b) {
  return (a < b) ? a : b;
}

template <typename T, uint32_t N>
struct Vector {
  private:
      T data[N];
      uint32_t entriesAdded = 0;

  public:
  void push_back(T element, appendType type = appendType::cycle) {
    if (type == cycle) {
        uint32_t latest = entriesAdded % N;
        data[latest] = element;
        entriesAdded++;
    }
    else if (type == ignore) {
      if (entriesAdded < N) {
        data[entriesAdded] = element;
        entriesAdded++;
      }
    }
  }

  T operator[](uint32_t idx) const {
    return data[(entriesAdded + 2*N - 1 - idx) % N]; 
  }

  struct VectorReturnObject {
    bool success = false;
    T value;
  };

  VectorReturnObject get_value(uint32_t idx) const {
    VectorReturnObject ret_value;
    if (idx >= entriesAdded) {
      return ret_value;
    }
    else {
      ret_value.success = true;
      ret_value.value = (*this)[idx];
      return ret_value;
    }
  }

  uint32_t size() const {
    return vec_min(entriesAdded, N);
  }

  void reset() { 
    entriesAdded = 0; 
    for (uint32_t i = 0; i < N; i++) {
      data[i] = T{};
    }
  }
};

template <typename T, uint32_t N>
struct TimedVector {
  public:
    struct DataPoint {
      T value;
      uint32_t time;
    };

    void push_back(DataPoint element, appendType type = appendType::cycle) {
      if (type == cycle) {
          uint32_t latest = entriesAdded % N;
          data[latest] = element;
          entriesAdded++;
      }
      else if (type == ignore) {
        if (entriesAdded < N) {
          data[entriesAdded] = element;
          entriesAdded++;
        }
      }
    }

    // Default to using builtin millis() arduino function 
    // if push_back with element only.
    void push_back(T element, appendType type = appendType::cycle) {
      push_back({element, (uint32_t)millis()}, type);
    }

    DataPoint operator[](uint32_t idx) const {
      return data[(entriesAdded + 2*N - 1 - idx) % N]; 
    }

    struct VectorReturnObject {
      bool success = false;
      DataPoint value;
    };

    VectorReturnObject get_value(uint32_t idx) const {
      VectorReturnObject ret_value;
      if (idx >= entriesAdded) {
        return ret_value;
      }
      else {
        ret_value.success = true;
        ret_value.value = (*this)[idx];
        return ret_value;
      }
    }

    uint32_t size() const {
      return vec_min(entriesAdded, N);
    }

    void reset() { 
      entriesAdded = 0; 
      for (uint32_t i = 0; i < N; i++) {
        data[i] = {}; // Empty struct
      }
    }

  private:
    DataPoint data[N];
    uint32_t entriesAdded = 0;
};

#endif
