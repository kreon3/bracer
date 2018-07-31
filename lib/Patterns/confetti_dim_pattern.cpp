#include "confetti_dim_pattern.h"
#include <FastLED.h>
#include <Arduino.h>

void ConfettiDimPattern::start() {
  Pattern::start();
  set_all(0x000000);
}

void ConfettiDimPattern::led_step(const sensors_event_t &sensor_event) {
  fadeToBlackBy(display_->leds(), display_->max_index(), 10);

  // Get new hues. Overflow is intended.
  uint8_t hue = sensor_event.orientation.x
      + sensor_event.orientation.y
      + sensor_event.orientation.z;
  // TODO: This should be a utility function.
  float accel =
      sensor_event.acceleration.x +
      sensor_event.acceleration.y +
      sensor_event.acceleration.z;

  uint32_t pos = rand() % display_->max_index();
  uint32_t pos_n1 = pos == 0 ? 0 : pos - 1;
  uint32_t pos_p1 = pos == display_->max_index() ? display_->max_index()
                                                 : pos + 1;

  // uint32_t pos = 2;
  display_->leds()[pos].setHSV(hue, 255, accel);
  display_->leds()[pos_n1].setHSV(hue, 255, accel);
  display_->leds()[pos_p1].setHSV(hue, 255, accel);

}
