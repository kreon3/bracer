#ifndef LIB_PATTERNS_LED_DISPLAY_H_
#define LIB_PATTERNS_LED_DISPLAY_H_

#include <FastLED.h>

// Helper to make the LEDs a grid.
class LedDisplay {
public:
  LedDisplay(CRGB *leds, uint8_t width, uint8_t height)
      : leds_(leds), width_(width), height_(height) {}

  void set_color(uint8_t column, uint8_t row, CHSV color);
  void set_color(uint8_t column, uint8_t row, CRGB color);

  uint8_t max_index();

private:
  uint8_t lookup_index(uint8_t column, uint8_t row);
  CRGB *leds_;
  uint8_t width_;
  uint8_t height_;
};

#endif // LIB_PATTERNS_LED_DISPLAY_H_
