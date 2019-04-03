#include "cuttlefish_pattern.h"

#include <Adafruit_BNO055.h>
#include <FastLED.h>
#include <Logging.h>
#include "sensor_util.h"

CuttleFishPattern::CuttleFishPattern(LedDisplay *display, unsigned long step_frequency_ms)
  : Pattern(display, step_frequency_ms), position_(-2), pattern_length_(8.) {
}

void CuttleFishPattern::start() {
  Pattern::start();
  set_all(0x000000);
}

void CuttleFishPattern::led_step(const sensors_event_t &sensor_event,
                           Adafruit_BNO055 &bno_sensor) {
  // Position is considered a 32 length row, going +/-2 over the 28.

  uint16_t intensity = get_linear_intensity(bno_sensor);
  intensity = min(255, intensity * 6);
  float movement = 0.25 + (intensity / 255.);
  position_ -= movement;  // Targeting moving 8 pixels a second.

  if (position_ < -2) position_ = position_ + 32;

  hue_a_ = sensor_event.orientation.x + sensor_event.orientation.y;
  hue_b_ = sensor_event.orientation.y + sensor_event.orientation.z;

  draw_row(0, -3);
  draw_row(1, 0);
  draw_row(2, -1);
  draw_row(3, -4);
}

void CuttleFishPattern::draw_row(uint8_t row, float position_offset) {
  float apex = position_ + position_offset;
  for (uint8_t c = 0; c < display_->width(); ++c) {
    // Generate an color coefficient in the range of +/- 0.5 removing
    // space in between.
    // Ranges:
    // 0-64: colorb
    // 64-192: no color
    // 192-255: colora
    uint8_t coefficient = sin8_avr((apex + c) * 16);
    if (coefficient > 192) {
      // LOG(INFO, "set_color_a ");
      // LOG(INFO, coefficient - 192);
      display_->set_color(c, row, CHSV(hue_a_, 255, (coefficient - 192)));
    } else if (coefficient < 64) {
      // LOG(INFO, "set_color_b ");
      // LOG(INFO, 64 - coefficient);
      display_->set_color(c, row, CHSV(hue_b_, 255, (64 - coefficient)));
    } else {
      display_->set_color(c, row, CHSV(0, 0, 0));
    }
  }
}
