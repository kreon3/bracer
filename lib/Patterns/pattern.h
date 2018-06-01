#ifndef LIB_PATTERNS_PATTERN_H_
#define LIB_PATTERNS_PATTERN_H_

#include "led_display.h"
#include <Adafruit_BNO055.h>

class Pattern {
public:
  Pattern(LedDisplay *display, unsigned long step_frequency_ms);
  // Starts the pattern.
  virtual void start() = 0;
  // Update the pattern.
  virtual void update(unsigned long millis,
                      const sensors_event_t &sensor_event);

  // Perform one step of the led pattern.
  virtual void led_step(const sensors_event_t &sensor_event) = 0;

  virtual void set_all(CRGB color);

protected:
  LedDisplay *display_;
  unsigned long step_frequency_ms_;
  unsigned long last_led_update_;
};

#endif // LIB_PATTERNS_PATTERN_H_
