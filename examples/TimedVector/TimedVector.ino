#include <Vectorial.h>

// Create a TimedVector to store temperature readings with timestamps
// Capacity: 5 readings
TimedVector<float, 5> temperatureLog;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // Wait for serial monitor
  }

  Serial.println("=== TimedVector Demo ===");
  Serial.println();

  // Test 1: Push values (timestamps captured automatically via millis())
  Serial.println("Test 1: Adding temperature readings");
  temperatureLog.push_back(22.5);
  delay(100); // Small delay to get different timestamps
  temperatureLog.push_back(22.7);
  delay(100);
  temperatureLog.push_back(22.6);
  delay(100);
  temperatureLog.push_back(22.8);
  delay(100);
  temperatureLog.push_back(22.9);

  Serial.print("Buffer size: ");
  Serial.println(temperatureLog.size()); // Should be 5

  // Test 2: Access elements (index 0 = most recent)
  Serial.println();
  Serial.println("Test 2: Reading back with timestamps");
  for (uint32_t i = 0; i < temperatureLog.size(); i++) {
    DataPoint dp = temperatureLog[i];
    Serial.print("Index ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(dp.value);
    Serial.print("°C (recorded at ");
    Serial.print(dp.time);
    Serial.println(" ms)");
  }

  // Test 3: Demonstrate cycling (6th push overwrites oldest)
  Serial.println();
  Serial.println("Test 3: Cycling behavior");
  temperatureLog.push_back(23.0); // Should overwrite 22.5
  Serial.print("Size after push: ");
  Serial.println(temperatureLog.size()); // Still 5

  DataPoint oldest = temperatureLog[4];
  Serial.print("Newest (index 0): ");
  Serial.println(temperatureLog[0].value);
  Serial.print("Oldest (index 4): ");
  Serial.println(oldest.value);
  Serial.println("(Should be 22.6 - 22.5 was overwritten)");

  // Test 4: Safe access with get_value()
  Serial.println();
  Serial.println("Test 4: Safe access with get_value()");
  auto result = temperatureLog.get_value(10); // Invalid index
  if (result.success) {
    Serial.println("This shouldn't print");
  } else {
    Serial.println("Correctly detected invalid index!");
  }

  result = temperatureLog.get_value(2);
  if (result.success) {
    Serial.print("Valid access - value: ");
    Serial.print(result.value.value);
    Serial.print(", time: ");
    Serial.println(result.value.time);
  }

  // Test 5: Reset functionality
  Serial.println();
  Serial.println("Test 5: Reset");
  temperatureLog.reset();
  Serial.print("Size after reset: ");
  Serial.println(temperatureLog.size()); // Should be 0

  // Test 6: Manual DataPoint push (with custom timestamp)
  Serial.println();
  Serial.println("Test 6: Manual DataPoint push");
  temperatureLog.push_back({25.0, 5000}); // Push with explicit time
  DataPoint dp = temperatureLog[0];
  Serial.print("Value: ");
  Serial.print(dp.value);
  Serial.print(", time: ");
  Serial.println(dp.time); // Should be 5000

  // Test 7: Ignore mode
  Serial.println();
  Serial.println("Test 7: Ignore mode (fill then ignore)");
  // Fill the buffer completely
  for (int i = 0; i < 5; i++) {
    temperatureLog.push_back(20.0 + i);
  }
  Serial.print("Full buffer size: ");
  Serial.println(temperatureLog.size());

  // Try to add with ignore mode - should not change anything
  temperatureLog.push_back(99.9, appendType::ignore);
  bool unchanged = (temperatureLog[0].value == 24.0);
  Serial.print("Buffer unchanged after ignore? ");
  Serial.println(unchanged ? "YES" : "NO");

  Serial.println();
  Serial.println("=== Demo Complete ===");
}

void loop() {
  // Nothing to do here - demo runs once in setup
}
