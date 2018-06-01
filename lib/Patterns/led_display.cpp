#include "led_display.h"

uint8_t LedDisplay::lookup_index(uint8_t column, uint8_t row) {
  if ((row & 1) == 0) {
    // Even rows have incrementing index.
    return row * width_ + column;
  } else {
    // Odd numbered rows are inverted in index.
    return (row + 1) * width_ - column - 1;
  }
}

uint8_t LedDisplay::max_index() { return width_ * height_ - 1; }

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
    Serial.print("Out of bounds index ");
    Serial.println(i);
  }
}

void LedDisplay::show() { FastLED.show(); }
