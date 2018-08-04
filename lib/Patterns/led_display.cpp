#include "led_display.h"
#include <Logging.h>

LedDisplay::LedDisplay(CRGB *leds, uint8_t width, uint8_t height)
    : leds_(leds), width_(width), height_(height) {}

CRGB *LedDisplay::get_pixel(uint8_t column, uint8_t row) {
  uint8_t i = lookup_index(column, row);
  // Check for out of bounds.
  if (i <= max_index()) {
    return &(leds_[i]);
  } else {
    LOGF(ERROR, "Out of bounds index ");
    LOGLN(ERROR, i);
    return nullptr;
  }
}

void LedDisplay::set_color(uint8_t column, uint8_t row, CHSV color) {
  uint8_t i = lookup_index(column, row);
  if (i <= max_index()) {
    leds_[i] = color;
  }
}
void LedDisplay::set_color(uint8_t column, uint8_t row, CRGB color) {
  uint8_t i = lookup_index(column, row);
  // Check for out of bounds.
  if (i <= max_index()) {
    leds_[i] = color;
  } else {
    LOGF(ERROR, "Out of bounds index ");
    LOGLN(ERROR, i);
  }
}

void LedDisplay::show() { FastLED.show(); }

uint8_t LedDisplay::max_index() const { return width_ * height_ - 1; }
uint8_t LedDisplay::width() const { return width_; }
uint8_t LedDisplay::height() const { return height_; }
uint8_t LedDisplay::cols() const { return width_; }
uint8_t LedDisplay::rows() const { return height_; }

uint8_t LedDisplay::lookup_index(uint8_t column, uint8_t row) const {
  if ((row & 1) == 0) {
    // Even rows have incrementing index.
    return row * width_ + column;
  } else {
    // Odd numbered rows are inverted in index.
    return (row + 1) * width_ - column - 1;
  }
}

CRGB* LedDisplay::leds() {
  return leds_;
}
