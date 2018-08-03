#ifndef CONFETTI_DIM_PATTERN_H_
#define CONFETTI_DIM_PATTERN_H_

#include "pattern.h"

class ConfettiDimPattern : public Pattern {
public:
  ConfettiDimPattern(LedDisplay *display, unsigned long step_frequency_ms)
      : Pattern(display, step_frequency_ms) {}
  void start() override;

  void led_step(const sensors_event_t &sensor_event,
                Adafruit_BNO055 &bno_sensor) override;

private:
};

#endif // CONFETTI_DIM_PATTERN_H_
