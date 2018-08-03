#ifndef GYRO_RAINBOW_PATTERN_H_
#define GYRO_RAINBOW_PATTERN_H_

#include "pattern.h"

class GyroRainbowPattern : public Pattern {
public:
  GyroRainbowPattern(LedDisplay *display, unsigned long step_frequency_ms)
      : Pattern(display, step_frequency_ms) {}
  void start() override;

  void led_step(const sensors_event_t &sensor_event,
                Adafruit_BNO055 &bno_sensor) override;

private:
};

#endif // GYRO_RAINBOW_PATTERN_H_
