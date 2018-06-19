
#include "bars_pattern.h"
#include <FastLED.h>
#include <Logging.h>


BarsPattern::BarsPattern(
  LedDisplay *display,
  unsigned long step_frequency_ms)
    : Pattern(display, step_frequency_ms) {

}

void BarsPattern::start() {
  Pattern::start();
  set_all(0x000000);

  for (uint8_t i = 0; i < NUM_BARS; ++i) {
    initBar(&bars_[i], i);
  }
}

void BarsPattern::led_step(const sensors_event_t &sensor_event) {
  for (uint8_t i = 0; i < NUM_BARS; ++i) {
    stepBar(&bars_[i]);
  }
}

void BarsPattern::initBar(BarsPattern::Bar *bar, uint8_t row) {
  uint8_t col = random(PIXEL_WIDTH / 2) + PIXEL_WIDTH / 4;
  bar->reset(
    random(255),
    255,
    random(150, 255),
    0,
    random(3, 20),
    true,
    row,
    col,
    random(15, 30));
  LOG(INFO, "New bar at ");
  LOG(INFO, row);
  LOG(INFO, ", ");
  LOGLN(INFO, col);
}

void BarsPattern::stepBar(BarsPattern::Bar *bar) {
  int16_t max = bar->max_value_;
  int16_t curr = bar->current_value_;
  int16_t next = curr;
  if (bar->increasing_) {
    next += bar->change_per_step_;
    if (next > max) {
      bar->increasing_ = false;
      next = max - (next - max);
    }
  } else {
    next -= bar->change_per_step_;
    if (next <= 0) {
      setRowColor(bar->row_, 0x000000);
      initBar(bar, bar->row_);
      return;
    }
  }
  bar->current_value_ = next;
  drawBar(bar);

}

void BarsPattern::setRowColor(uint8_t row, CRGB color) {
  for (size_t i = 0; i < PIXEL_WIDTH; ++i) {
    display_->set_color(i, row, color);
  }
}

void BarsPattern::drawBar(Bar* bar) {
  for (int i = 0; i < PIXEL_WIDTH; ++i) {
    int col_int = bar->col_;
    int distance = abs(col_int - i);

    uint8_t value = max(bar->current_value_ - distance * bar->taper_, 0);
    display_->set_color(
      i, bar->row_,
      CHSV(bar->hue_, bar->saturation_, value));
  }
}


void BarsPattern::Bar::reset(
  uint8_t hue,
  uint8_t saturation,
  uint8_t max_value,
  uint8_t current_value,
  uint8_t change_per_step,
  bool increasing,
  uint8_t row,
  uint8_t col,
  uint8_t taper) {

  hue_ = hue;
  saturation_ = saturation;
  max_value_ = max_value;
  current_value_ = current_value;
  change_per_step_ = change_per_step;
  increasing_ = increasing;
  row_ = row;
  col_ = col;
  taper_ = taper;
  }
