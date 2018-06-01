#ifndef LIB_PATTERNS_H_
#define LIB_PATTERNS_H_

#include "led_display.h"
#include "pattern.h"

class Patterns {
public:
  Patterns(CRGB *leds, uint8_t width, uint8_t height);
  enum PatternType {
    UNLIT,
    GYRO_RAINBOW,
    RANDOM_RAINBOW,
    SPARKLE,
    BARS,
    MAX_PATTERN };

  void set_pattern(PatternType pattern);
  PatternType get_pattern();
  void next_pattern();

  void update(unsigned long millis, const sensors_event_t &sensor_event);

private:
  LedDisplay display_;
  PatternType current_pattern_type_;
  Pattern *patterns_[MAX_PATTERN];
};

#endif // LIB_LED_DISPLAY_H_
