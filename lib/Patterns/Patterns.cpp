#include "Patterns.h"

#include "bars_pattern.h"
#include "confetti_dim_pattern.h"
#include "fire_pattern.h"
#include "gyro_rainbow_pattern.h"
#include "random_rainbow_pattern.h"
#include "sparkle_pattern.h"
#include "unlit_pattern.h"
#include <Logging.h>

Patterns::Patterns(CRGB *leds, uint8_t width, uint8_t height)
    : display_(leds, width, height), current_pattern_type_(FIRE) {
  patterns_[UNLIT] = new UnlitPattern(&display_);
  patterns_[GYRO_RAINBOW] = new GyroRainbowPattern(&display_, 20);
  patterns_[RANDOM_RAINBOW] = new RandomRainbowPattern(&display_, 20);
  patterns_[SPARKLE] = new SparklePattern(&display_, 20);
  patterns_[BARS] =  new BarsPattern(&display_, 20);
  patterns_[CONFETTI] = new ConfettiDimPattern(&display_, 20);
  patterns_[FIRE] = new FirePattern(&display_, 30);
  // patterns_[BLUE_FIRE] = patterns_[FIRE];
  set_pattern(current_pattern_type_);
}

Patterns::PatternType Patterns::get_pattern() { return current_pattern_type_; }

void Patterns::set_pattern(PatternType pattern) {
  LOGF(INFO, "Setting pattern to ");
  LOGLN(INFO, pattern);
  current_pattern_type_ = pattern;
  patterns_[pattern]->start();
}

void Patterns::update(unsigned long millis,
                      const sensors_event_t &sensor_event,
                      Adafruit_BNO055 &bno_sensor) {
  patterns_[get_pattern()]->update(millis, sensor_event, bno_sensor);
}

void Patterns::next_pattern() {

  PatternType curr_pattern = get_pattern();
  size_t pattern_int = curr_pattern;
  ++pattern_int;
  if (pattern_int >= MAX_PATTERN) {
    pattern_int = 0;
  }
  PatternType next_pattern = static_cast<PatternType>(pattern_int);
  LOGF(INFO, "Switching to next_pattern ");
  LOG(INFO, next_pattern);
  LOGF(INFO, " from pattern ");
  LOG(INFO, curr_pattern);

  LOGF(INFO, " max patterns ");
  LOGLN(INFO, MAX_PATTERN);
  set_pattern(next_pattern);
}
