#ifndef VECTORIALAVIONICS_H_
#define VECTORIALAVIONICS_H_

#include <cstdint>

#ifndef CI
#include <Arduino.h>
#include <Arduino_BMI270.h>
#include <Servo.h>
#include <Wire.h>

// Create the global BMI270 object instance
BMI270Class BMI;
#endif

#define clamp(value, low, high) ((value) < (low) ? (low) : ((value) > (high) ? (high) : (value)))
constexpr float kRadToDeg = 57.2957795131f;
class VectorialTVC {
public:
  enum class GyroModels {
    MPU6050,
    MPU9250,
    ICM20948,
    BMI270,
  };

  struct GyroAngles {
    float x;
    float y;
    float z;
  };

  struct ServoAngles {
    float x;
    float z;    
  };

  struct Telemetry {
    GyroAngles gyro;
    ServoAngles servo;
  };

  struct PIDGains {
    float k_p;
    float k_d;
    float k_i;
    float min_rad;
    float max_rad;
  };

  struct ServoSettings {
    uint8_t x_servo_pin;
    uint8_t z_servo_pin;
    uint8_t x_servo_midpt;
    uint8_t z_servo_midpt;
  };

  struct Config {
    PIDGains pid_params = {1, 0.2, 0.1, -0.1, 0.1};
    GyroModels gyro_model = VectorialTVC::GyroModels::BMI270;
    ServoSettings servo_settings = {4, 5, 90, 90};
    uint32_t millis_per_loop = 33;
  };

  public:
  VectorialTVC(const Config& config) {
#ifndef CI
    pinMode(config.servo_settings.x_servo_pin, OUTPUT);
    pinMode(config.servo_settings.z_servo_pin, OUTPUT);

    // Initialize the I2C physical bus once before checking models
    Wire.begin();

    switch (config.gyro_model) {
    case GyroModels::MPU6050:
      Wire.beginTransmission(0x68); // Default I2C Address
      Wire.write(0x6B);             // PWR_MGMT_1 register
      Wire.write(0x00);             // Clear sleep bit (Wake up)
      Wire.endTransmission();
      delay(100); // Wait for clock stabilization
      break;

    case GyroModels::MPU9250:
      Wire.beginTransmission(0x68); // Default I2C Address
      Wire.write(0x6B);             // PWR_MGMT_1 register
      Wire.write(0x00);             // Clear sleep bit
      Wire.endTransmission();
      delay(10);

      Wire.beginTransmission(0x68);
      Wire.write(0x6C); // PWR_MGMT_2 register
      Wire.write(0x00); // Enable all axes
      Wire.endTransmission();
      delay(100);
      break;

    case GyroModels::ICM20948:
      // Wake up main chip (User Bank 0)
      Wire.beginTransmission(0x68); // Default I2C Address
      Wire.write(0x06);             // PWR_MGMT_1 register on Bank 0
      Wire.write(0x01);             // Clear Sleep + Auto-select best clock
      Wire.endTransmission();
      delay(10);

      // Enable Gyro and Accelerometer paths
      Wire.beginTransmission(0x68);
      Wire.write(0x07); // PWR_MGMT_2 register on Bank 0
      Wire.write(0x00); // Enable all axes
      Wire.endTransmission();
      delay(100);
      break;

    case GyroModels::BMI270:
      if (!BMI.begin()) {
        Serial.println("Failed to initialize BMI270 sensor!");
        delay(100);
      }
      break;
    }

#endif
    // Now that the gyro is set, set up the servos
    ServoX.attach(config.servo_settings.x_servo_pin);
    ServoZ.attach(config.servo_settings.z_servo_pin);
    ServoX.write(config.servo_settings.x_servo_midpt);
    ServoZ.write(config.servo_settings.z_servo_midpt);

    // Save all settings
    config_intl.servo_settings = config.servo_settings;
    config_intl.gyro_model = config.gyro_model;
    config_intl.millis_per_loop = config.millis_per_loop;
    config_intl.pid_params = config.pid_params;
  };
  ~VectorialTVC() = default;

  GyroAngles ReadGyro() {
    GyroAngles angles = {0.0f, 0.0f, 0.0f};
    switch (config_intl.gyro_model) {
    case GyroModels::MPU6050:
      Wire.beginTransmission(0x68);
      Wire.write(0x43);
      Wire.endTransmission(false);

      Wire.requestFrom(0x68, (uint8_t)6);
      if (Wire.available() == 6) {
        angles.x =
            static_cast<float>((Wire.read() << 8) | Wire.read()) / 131.0f;
        angles.y =
            static_cast<float>((Wire.read() << 8) | Wire.read()) / 131.0f;
        angles.z =
            static_cast<float>((Wire.read() << 8) | Wire.read()) / 131.0f;
      }
      break;

    case GyroModels::MPU9250:
      Wire.beginTransmission(0x68);
      Wire.write(0x43);
      Wire.endTransmission(false);
      Wire.requestFrom(0x68, (uint8_t)6);
      if (Wire.available() == 6) {
        angles.x =
            static_cast<float>((Wire.read() << 8) | Wire.read()) / 131.0f;
        angles.y =
            static_cast<float>((Wire.read() << 8) | Wire.read()) / 131.0f;
        angles.z =
            static_cast<float>((Wire.read() << 8) | Wire.read()) / 131.0f;
      }
      break;
    case GyroModels::ICM20948:
      Wire.beginTransmission(0x68);
      Wire.write(0x33);
      Wire.endTransmission(false);
      Wire.requestFrom(0x68, (uint8_t)6);
      if (Wire.available() == 6) {
        angles.x =
            static_cast<float>((Wire.read() << 8) | Wire.read()) / 131.0f;
        angles.y =
            static_cast<float>((Wire.read() << 8) | Wire.read()) / 131.0f;
        angles.z =
            static_cast<float>((Wire.read() << 8) | Wire.read()) / 131.0f;
      }
      break;

    case GyroModels::BMI270:
      if (BMI.gyroscopeAvailable()) {
        BMI.readGyroscope(angles.x, angles.y, angles.z);
      }
      break;
    }

    currentAngles.x += angles.x * config_intl.millis_per_loop / 1000.0f;
    currentAngles.y += angles.y * config_intl.millis_per_loop / 1000.0f;
    currentAngles.z += angles.z * config_intl.millis_per_loop / 1000.0f;
    return currentAngles;
  }

  ServoAngles ComputeTvc() {
    integral_x += currentAngles.x * config_intl.millis_per_loop;
    float output_x = 0; float output_z = 0;
    if (!x_set) {
        prev_value_x = currentAngles.x;
        x_set=true;
    }
    output_x = config_intl.pid_params.k_p * currentAngles.x + config_intl.pid_params.k_i * integral_x + config_intl.pid_params.k_d * (currentAngles.x - prev_value_x) / config_intl.millis_per_loop;
    prev_value_x = currentAngles.x;
    
    integral_z += currentAngles.z * config_intl.millis_per_loop;
     if (!z_set) {
        prev_value_z = currentAngles.z;
        z_set=true;
    }
    output_z = config_intl.pid_params.k_p * currentAngles.z + config_intl.pid_params.k_i * integral_z + config_intl.pid_params.k_d * (currentAngles.z - prev_value_z) / config_intl.millis_per_loop;
    prev_value_z = currentAngles.z;
    ServoX.write(config_intl.servo_settings.servo_x_midpt + clamp(output_x, config_intl.pid_params.min_rad, config_intl.pid_params.max_rad) * kRadToDeg);
    ServoZ.write(config_intl.servo_settings.servo_z_midpt + clamp(output_z, config_intl.pid_params.min_rad, config_intl.pid_params.max_rad) * kRadToDeg);
    return {output_x, output_z};
  }

  Telemetry ControlLoop() {
    const GyroAngles gyro = ReadGyro();
    const ServoAngles servo = ComputeTvc();
    return {gyro, servo};
  }

  GyroAngles GetAngles() const { return currentAngles; }

private:
  Servo ServoX;
  Servo ServoZ;
  Config config_intl;
  float integral_x = 0;
  float prev_value_x = 0;
  float integral_z = 0;
  float prev_value_z = 0;
  bool x_set = false;
  bool z_set = false;

  GyroAngles currentAngles = {0, 0, 0};
};
