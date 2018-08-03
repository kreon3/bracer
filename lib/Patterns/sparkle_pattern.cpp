
#include "sparkle_pattern.h"
#include <Adafruit_BNO055.h>
#include <FastLED.h>
#include <Logging.h>


SparklePattern::SparklePattern(
  LedDisplay *display,
  unsigned long step_frequency_ms)
    : Pattern(display, step_frequency_ms) {

}

void SparklePattern::start() {
  Pattern::start();
  set_all(0x000000);

  for (uint8_t r = 0; r < SPARKLE_HEIGHT; ++r) {
    for (uint8_t c = 0; c < SPARKLE_WIDTH; ++c) {
      active_pixel_map_[r][c] = false;
    }
  }
  for (uint8_t i = 0; i < NUM_SPARKLES; ++i) {
    initSparkle(&sparkles_[i]);
  }
}

void SparklePattern::led_step(const sensors_event_t &sensor_event,
                              Adafruit_BNO055 &bno_sensor) {
  for (uint8_t i = 0; i < NUM_SPARKLES; ++i) {
    stepSparkle(&sparkles_[i]);
  }
}

void SparklePattern::initSparkle(SparklePattern::Sparkle *sparkle) {
  uint8_t row, col;
  pick_unused_pixel(&row, &col);
  active_pixel_map_[row][col] = true;
  sparkle->reset(
    random(255), random(200, 255), random(255), 0,
    random(5, 30), true, row, col);
  LOGF(VERBOSE, "New sparkle at ");
  LOG(VERBOSE, row);
  LOGF(VERBOSE, ", ");
  LOGLN(VERBOSE, col);
}

void SparklePattern::stepSparkle(SparklePattern::Sparkle *sparkle) {
  int16_t max = sparkle->max_value_;
  int16_t curr = sparkle->current_value_;
  int16_t next = curr;
  if (sparkle->increasing_) {
    next += sparkle->change_per_step_;
    if (next > max) {
      sparkle->increasing_ = false;
      next = max - (next - max);
    }
  } else {
    next -= sparkle->change_per_step_;
    if (next <= 0) {
      active_pixel_map_[sparkle->row_][sparkle->col_] = false;
      display_->set_color(sparkle->col_, sparkle->row_, 0x000000);
      initSparkle(sparkle);
      return;
    }
  }
  sparkle->current_value_ = next;
  display_->set_color(
    sparkle->col_, sparkle->row_,
    CHSV(sparkle->hue_, sparkle->saturation_, sparkle->current_value_));
}

void SparklePattern::pick_unused_pixel(uint8_t *row, uint8_t* col) {
  do {
    *row = random(SPARKLE_HEIGHT);
    *col = random(SPARKLE_WIDTH);
  } while (active_pixel_map_[*row][*col]);
}

void SparklePattern::Sparkle::reset(
  uint8_t hue,
  uint8_t saturation,
  uint8_t max_value,
  uint8_t current_value,
  uint8_t change_per_step,
  bool increasing,
  uint8_t row,
  uint8_t col) {

  hue_ = hue;
  saturation_ = saturation;
  max_value_ = max_value;
  current_value_ = current_value;
  change_per_step_ = change_per_step;
  increasing_ = increasing;
  row_ = row;
  col_ = col;
  }
