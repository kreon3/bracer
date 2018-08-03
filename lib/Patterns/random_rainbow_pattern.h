#ifndef RANDOM_RAINBOW_PATTERN_H_
#define RANDOM_RAINBOW_PATTERN_H_

#include "pattern.h"

class RandomRainbowPattern : public Pattern {
public:
  RandomRainbowPattern(LedDisplay *display, unsigned long step_frequency_ms)
      : Pattern(display, step_frequency_ms) {}
  void start() override;

  void led_step(const sensors_event_t &sensor_event,
                Adafruit_BNO055 &bno_sensor) override;

private:
  uint8_t hue0_ = 0;
  uint8_t hue1_ = 0;
  uint8_t hue2_ = 0;
  uint8_t hue3_ = 0;
};

#endif // RANDOM_RAINBOW_PATTERN_H_
