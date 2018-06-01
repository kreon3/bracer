
#include "Patterns.h"
#include "gyro_rainbow_pattern.h"
#include "unlit_pattern.h"
#include <Logging.h>

Patterns::Patterns(CRGB *leds, uint8_t width, uint8_t height)
    : display_(leds, width, height), current_pattern_type_(GYRO_RAINBOW) {
  // Pattern *unlit = new UnlitPattern(&display_);
  patterns_[UNLIT] = new UnlitPattern(&display_);
  patterns_[GYRO_RAINBOW] = new GyroRainbowPattern(&display_, 20);
  patterns_[SPARKLE] = new UnlitPattern(&display_);
  patterns_[BARS] = new GyroRainbowPattern(&display_, 20);
  set_pattern(current_pattern_type_);
}

Patterns::PatternType Patterns::get_pattern() { return current_pattern_type_; }

void Patterns::set_pattern(PatternType pattern) {
  LOG(INFO, "Setting pattern to ");
  LOGLN(INFO, pattern);
  current_pattern_type_ = pattern;
  patterns_[pattern]->start();
}

void Patterns::update(unsigned long millis,
                      const sensors_event_t &sensor_event) {
  patterns_[get_pattern()]->update(millis, sensor_event);
}

void Patterns::next_pattern() {
  PatternType t = get_pattern();
  if (t >= MAX_PATTERN) {
    set_pattern(static_cast<PatternType>(0));
  } else {
    set_pattern(static_cast<PatternType>(static_cast<uint8_t>(t) + 1));
  }
}
