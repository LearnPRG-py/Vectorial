#ifndef VECTORIALMATH_H_
#define VECTORIALMATH_H_

#include "Vectorial.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <math.h>

struct TimedVectorMath {
  template <typename T, uint32_t N>
  static float moving_mean(const TimedVector<T, N> &vec, uint32_t elements) {
    assert(elements <= N);
    assert(elements > 0);
    T sum = 0;
    for (uint32_t i = 0; i < elements; i++) {
      sum += vec[i].value;
    }
    return static_cast<float>(sum) / elements;
  }
  template <typename T, uint32_t N>
  static float ema(const TimedVector<T, N> &vec, uint32_t elements,
                   float alpha = 0) {
    assert(elements <= N);
    assert(elements > 0);
    float ema = 0;
    if (alpha == 0) {
      alpha = 2.0f / (1 + elements);
    }
    for (int64_t i = static_cast<int64_t>(elements) - 1; i >= 0; i--) {
      if (i == static_cast<int64_t>(elements) - 1) {
        ema = vec[static_cast<uint32_t>(i)].value; // oldest point in window
      } else {
        ema = ema * (1 - alpha) + vec[static_cast<uint32_t>(i)].value * alpha;
      }
    }
    return ema;
  }
  template <typename T, uint32_t N>
  static float derive(const TimedVector<T, N> &vec) {
    if (vec.size() < 2) {
      return 0.0f;
    }
    return static_cast<float>(vec[0].value - vec[1].value) * 1000 /
           (vec[0].time - vec[1].time);
  }
  template <typename T, uint32_t N>
  static float integrate(const TimedVector<T, N> &vec, uint32_t elements) {
    // When the window covers the full buffer, there's no vec[N] to pair with
    // the oldest point — estimate its contribution standalone instead of
    // dropping it. Also serves as built-in anti-windup.
    assert(elements <= N);
    assert(elements > 0);
    float integral = 0;
    if (elements == N) {
      integral = vec[elements - 1].value * vec[elements - 1].time *
                 (0.001f); // Estimate of state lost before timed vector.
    }
    for (uint32_t i = 0; i < std::min(elements, N - 1); i++) {
      integral += vec[i].value * (0.001f) * (vec[i].time - vec[i + 1].time);
    }
    return integral;
  }
};

#endif // VECTORIALMATH_H_
