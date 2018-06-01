
#include "gyro_rainbow_pattern.h"
#include <FastLED.h>

void GyroRainbowPattern::start() {
  Pattern::start();
  set_all(0x001100);
}

void GyroRainbowPattern::led_step(const sensors_event_t &sensor_event) {

  // TODO: This could be made significantly faster with memmove if needed.
  for (int r = 0; r < display_->height(); ++r) {
    for (int c = display_->width() - 1; c > 0; --c) {
      *display_->get_pixel(c, r) = *display_->get_pixel(c - 1, r);
    }
  }

  // Get new hues. Overflow is intended.
  uint8_t hue0 = sensor_event.orientation.x + sensor_event.orientation.y;
  uint8_t hue1 = sensor_event.orientation.y + sensor_event.orientation.z;
  uint8_t hue2 = sensor_event.orientation.z + sensor_event.orientation.x;
  uint8_t hue3 = sensor_event.orientation.x + sensor_event.orientation.y +
                 sensor_event.orientation.z;
  *display_->get_pixel(0, 0) = CHSV(hue0, 255, 255);
  *display_->get_pixel(0, 1) = CHSV(hue1, 255, 255);
  *display_->get_pixel(0, 2) = CHSV(hue2, 255, 255);
  *display_->get_pixel(0, 3) = CHSV(hue3, 255, 255);
}
