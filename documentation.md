# Vectorial Library - Comprehensive Documentation

## Introduction

Vectorial brings vector-style append operations with rotating buffers to Arduino/C++ environments. It provides two container types: `Vector` for generic fixed-capacity arrays and `TimedVector` for time-series logging, both with zero dynamic memory allocation.

## Core Concepts

### Circular Buffer Mechanics
Both classes use a fixed-size circular buffer with an `entriesAdded` counter. When the buffer reaches capacity `N`, new elements either overwrite the oldest (`appendType::cycle`) or are discarded (`appendType::ignore`). The `size()` method returns the number of valid elements (`min(entriesAdded, N)`).

### Indexing Order
Index `0` always refers to the most recently appended element. Higher indices access older values. This reverse ordering allows immediate access to the latest data without modulo arithmetic.

## Vector Class

### Template Parameters
- `T`: Type of stored elements.
- `N`: Compile-time capacity (positive integer).

### Construction
Default constructor zero-initializes the internal buffer and sets `entriesAdded` to 0.

### Member Functions

#### `void push_back(T element, appendType type = appendType::cycle)`
Appends `element` to the buffer.
- `cycle` (default): Overwrites oldest element if buffer full.
- `ignore`: Skips insertion if buffer full.

#### `T operator[](uint32_t idx) const`
Returns element at index `idx` (0 = most recent). No bounds checking; out-of-range results in undefined behavior.

#### `VectorReturnObject get_value(uint32_t idx) const`
Safe element access. Returns an object: `{ bool success; T value; }`. `success` is `false` if `idx >= size()`.

#### `uint32_t size() const`
Number of valid elements currently stored ( ≤ N ).

#### `void reset()`
Clears buffer: sets `entriesAdded` to 0 and zeroes all storage.

### Return Type
```cpp
template<typename T>
struct VectorReturnObject {
    bool success;
    T value;
};
```

### Example: Basic Usage

```cpp
#include <Vectorial.h>

Vector<int, 3> buffer;

void setup() {
  buffer.push_back(5);
  int list[5] = {1, 2, 3, 4, 5};
  for (int i = 0; i < 5; i++) {
    buffer.push_back(list[i]); // Cycles: only last 3 values kept
  }
  Serial.begin(9600);
  while (!Serial) {}

  Serial.println("Test 1 - latest value:");
  Serial.println(buffer[0]); // 5

  Serial.println("Test 2 - all values (most recent first):");
  for (uint32_t i = 0; i < buffer.size(); i++) {
    Serial.println(buffer[i]);
  }

  buffer.reset();
  buffer.push_back(84);
  Serial.println("Test 3 - after reset:");
  Serial.println(buffer[0]); // 84

  Serial.println("Test 4 - safe access:");
  auto ret = buffer.get_value(0);
  if (ret.success) {
    Serial.println(ret.value);
  } else {
    Serial.println("Failure state!");
  }
}

void loop() {}
```

## TimedVector Class

### Template Parameters
- `T`: Type of the stored value.
- `N`: Capacity.

### DataPoint
Each element is a `DataPoint` struct pairing a value with a timestamp.

```cpp
struct DataPoint {
    T value;       // Data value
    uint32_t time; // Timestamp from millis() (milliseconds)
};
```

### Construction
Default constructor; no parameters.

### Member Functions

#### `void push_back(T value, appendType type = appendType::cycle)`
Appends `value` with current timestamp obtained from `millis()`.

#### `void push_back(DataPoint dp, appendType type = appendType::cycle)`
Appends a full `DataPoint`, allowing custom timestamps.

#### `DataPoint operator[](uint32_t idx) const`
Returns the `DataPoint` at index `idx`.

#### `VectorReturnObject<DataPoint> get_value(uint32_t idx) const`
Safe access: returns `{ success, value }` where `value` is a `DataPoint`.

#### `uint32_t size() const` and `void reset()` have the same semantics as in `Vector`.

### Example: Temperature Logger

```cpp
#include <Vectorial.h>

TimedVector<float, 5> temperatureLog;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  // Log temperatures with auto timestamps
  temperatureLog.push_back(22.5);
  delay(100);
  temperatureLog.push_back(22.7);
  delay(100);
  temperatureLog.push_back(22.6);
  delay(100);
  temperatureLog.push_back(22.8);
  delay(100);
  temperatureLog.push_back(22.9);

  Serial.print("Buffer size: ");
  Serial.println(temperatureLog.size()); // 5

  // Display readings with timestamps
  for (uint32_t i = 0; i < temperatureLog.size(); i++) {
    DataPoint dp = temperatureLog[i];
    Serial.print("Reading ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(dp.value);
    Serial.print("°C at ");
    Serial.println(dp.time);
  }

  // Overwrite oldest when full (cycling)
  temperatureLog.push_back(23.0);
  Serial.println("After push (6th reading):");
  Serial.print("Newest: "); Serial.println(temperatureLog[0].value);
  Serial.print("Oldest: "); Serial.println(temperatureLog[4].value);

  // Safe access
  auto result = temperatureLog.get_value(10);
  if (!result.success) {
    Serial.println("Caught invalid index!");
  }

  // Reset
  temperatureLog.reset();
  Serial.print("Size after reset: ");
  Serial.println(temperatureLog.size());
}

void loop() {}
```

## Common Operations

### Append Types
- `appendType::cycle` (default): Overwrites oldest element when buffer is full.
- `appendType::ignore`: Discards new element if buffer is full.

### Memory and Performance
- Fixed capacity `N` is a template parameter; memory is allocated statically.
- No heap allocation, suitable for embedded systems.
- All operations are O(1) and consist of simple arithmetic and memory stores.

### Thread Safety
Not thread-safe. Designed for single-threaded Arduino contexts. Concurrent access requires external synchronization.

## API Quick Reference

| Feature               | Vector                                  | TimedVector                                       |
|-----------------------|------------------------------------------|---------------------------------------------------|
| Element type          | `T`                                      | `DataPoint{ T value; uint32_t time; }`            |
| Push (auto-time)      | `push_back(T)`                           | `push_back(T)` (uses `millis()`)                  |
| Push (explicit)       | `push_back(T)`                           | `push_back(DataPoint)`                            |
| Safe getter           | `get_value(idx)` → `{ success, value }`  | `get_value(idx)` → `{ success, DataPoint }`      |
| Size                  | `size()`                                 | `size()`                                          |
| Reset                 | `reset()`                                | `reset()`                                         |
| Capacity              | `N` (template argument)                 | `N` (template argument)                           |

## Limitations

- Fixed maximum capacity; cannot be resized at runtime.
- No iterator support; random access only via index.
- Index 0 is most recent (reverse chronological order), which may be counter-intuitive.
- On AVR, large `N` consumes precious RAM; choose the smallest sufficient capacity.

## Implementation Notes

Both classes are defined entirely in `src/Vectorial.h`. The library conditionally includes `Arduino.h` for `millis()`. When compiled in non-Arduino environments (e.g., host-based tests), it falls back to C++ `<chrono>` to provide a compatible `millis()` function.

## License
See the LICENSE file in the repository root.
