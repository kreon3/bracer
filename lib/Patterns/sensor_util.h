#ifndef SENSOR_UTIL_H_
#define SENSOR_UTIL_H_

#include <Adafruit_BNO055.h>

// Returns an intensity from 0-255 from the linear acceleration.
uint8_t get_linear_intensity(Adafruit_BNO055 &sensor);

// Logs the normalized gravity vector.
void log_gravity(Adafruit_BNO055 &sensor);

// Rough position of the bracer.
enum Position {
  // Pointed up.
  UP,
  // Pointed down.
  DOWN,
  // Roughly horizontal / parallel to the ground.
  HORIZONTAL
};

// Gets the rough position of the bracer.
Position get_position(Adafruit_BNO055 &sensor);

#endif // SENSOR_UTIL_H_
