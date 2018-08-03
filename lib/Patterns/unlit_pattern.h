#ifndef LIB_UNLIT_PATTERN_H_
#define LIB_UNLIT_PATTERN_H_

#include "pattern.h"

class UnlitPattern : public Pattern {
public:
  UnlitPattern(LedDisplay *display) : Pattern(display, 20) {}
  void start() override;

  void led_step(const sensors_event_t &sensor_event,
                Adafruit_BNO055 &bno_sensor) override;

private:
};

#endif // LIB_UNLIT_PATTERN_H_
