#include "sensor_util.h"

#include <Adafruit_BNO055.h>
#include <FastLED.h>
#include <Logging.h>

static double max_linear_accel = 0;
uint8_t get_linear_intensity(Adafruit_BNO055 &sensor) {
  imu::Vector<3> lin_accel_v
      = sensor.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  // Expected values for intensity should be 0-200. Expecting max linear
  // acceleration to be 4m/s.
  uint32_t intensity = lin_accel_v.magnitude() * 15 / 2;
  max_linear_accel = max(lin_accel_v.magnitude(), max_linear_accel);

  uint8_t result = min(intensity, 255);
  LOGF(INFO, "Intensity(");
  LOG(INFO, lin_accel_v.magnitude());
  LOGF(INFO, ", ");
  LOG(INFO, max_linear_accel);
  LOGF(INFO, "): ");
  LOGLN(INFO, result);
  return result;
}

void log_gravity(Adafruit_BNO055 &sensor) {
  imu::Vector<3> gravity
      = sensor.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
  gravity.normalize();
  LOGF(INFO, "Gravity: (");
  LOG(INFO, gravity.x());
  LOGF(INFO, ", ");
  LOG(INFO, gravity.y());
  LOGF(INFO, ", ");
  LOG(INFO, gravity.z());
  LOGLN(INFO, ")");
}

Position get_position(Adafruit_BNO055 &sensor) {
  imu::Vector<3> gravity
      = sensor.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
  gravity.normalize();
  // Rough sensor readings for normalized gravity, (x, y, z).
  // Up: (1, 0.01, -0.03)
  // Down: (-1, ..., ...)
  // Mid: (0.5 to -0.5, ..., ...)
  if (gravity.x() > 0.5) {
    return Position::UP;
  } else if (gravity.x() < -0.5) {
    return Position::DOWN;
  } else {
    return Position::HORIZONTAL;
  }
}
