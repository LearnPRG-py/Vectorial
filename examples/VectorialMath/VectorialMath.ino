#include <Vectorial.h>
#include <VectorialMath.h>

TimedVector<float, 5> gyroData; // Angular Velocity

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  // Actual data can come from reading the sensor, these are just sample data points.
  gyroData.push_back(1.35);
  delay(100);
  gyroData.push_back(1.26);
  delay(100);
  gyroData.push_back(1.41);
  delay(100);
  gyroData.push_back(1.34);
  delay(100);
  gyroData.push_back(1.52);
  // Simple moving average value of the gyro data based on last 5 values:
  float mean5 = TimedVectorMath::moving_mean(gyroData, 5);
  Serial.print("Mean of last 5 is: ");
  Serial.println(mean5);
  // Based on last 3 values:
  float mean3 = TimedVectorMath::moving_mean(gyroData, 3);
  Serial.print("Mean of last 3 is: ");
  Serial.println(mean3);
  // Exponential moving average:
  float emaVal = TimedVectorMath::ema(gyroData, 5);
  Serial.print("EMA is: ");
  Serial.println(emaVal);
  // Differentiate for angular acceleration:
  float angularAccel = TimedVectorMath::derive(gyroData);
  Serial.print("Angular acceleration is: ");
  Serial.println(angularAccel);
  // Integrate for angular position:
  float angularPosition = TimedVectorMath::integrate(gyroData, 5);
  Serial.print("Angular position is: ");
  Serial.println(angularPosition);
}

void loop {}
