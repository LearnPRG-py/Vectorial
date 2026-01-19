#include <Vectorial.h>

Vector<int, 3> buffer;

void setup() {
  buffer.push_back(5);
  int list[5] = {1, 2, 3, 4, 5};
  for (int i = 0; i < 5; i++) {
    buffer.push_back(list[i]);
  }
  Serial.begin(9600);
  while (!Serial) {
    // Wait
  }

  // Should give latest value (5)
  Serial.println(buffer[0]);

  // Returns 3 last entries added 3, 4 and 5 all in newlines.
  for (int i = 0; i < buffer.size(); i++) {
    Serial.println(buffer[i]);
  }

  buffer.reset();
  buffer.push_back(84);

  // Should give latest value (84)
  Serial.println(buffer[0]);
}
