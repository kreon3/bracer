#ifndef RANDOM_RAINBOW_PATTERN_H_
#define RANDOM_RAINBOW_PATTERN_H_

#include "pattern.h"

class RandomRainbowPattern : public Pattern {
public:
  RandomRainbowPattern(LedDisplay *display, unsigned long step_frequency_ms)
      : Pattern(display, step_frequency_ms) {}
  void start() override;

  void led_step(const sensors_event_t &sensor_event) override;

private:
};

#endif // RANDOM_RAINBOW_PATTERN_H_
