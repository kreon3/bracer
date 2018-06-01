
#include "random_rainbow_pattern.h"
#include <FastLED.h>

void RandomRainbowPattern::start() {
  Pattern::start();
  set_all(0x000011);
}

static uint8_t random_delta_hue(uint8_t hue) {
  return hue + 20 - random(40);
}

void RandomRainbowPattern::led_step(const sensors_event_t &sensor_event) {

  // TODO: This could be made significantly faster with memmove if needed.
  for (int r = 0; r < display_->height(); ++r) {
    for (int c = display_->width() - 1; c > 0; --c) {
      *display_->get_pixel(c, r) = *display_->get_pixel(c - 1, r);
    }
  }

  hue0_ = random_delta_hue(hue0_);
  hue1_ = random_delta_hue(hue1_);
  hue2_ = random_delta_hue(hue2_);
  hue3_ = random_delta_hue(hue3_);

  // Get new hues. Overflow is intended.
  display_->get_pixel(0, 0)->setHue(hue0_);
  display_->get_pixel(0, 1)->setHue(hue1_);
  display_->get_pixel(0, 2)->setHue(hue2_);
  display_->get_pixel(0, 3)->setHue(hue3_);
}
