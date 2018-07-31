#include "confetti_dim_pattern.h"
#include <FastLED.h>
#include <Arduino.h>
#include <Logging.h>

void ConfettiDimPattern::start() {
  Pattern::start();
  set_all(0x000000);
}

void ConfettiDimPattern::led_step(const sensors_event_t &sensor_event) {
  LOG(ERROR, F("led_step called on ConfettiDimPattern without bno_sensor"));
}

void ConfettiDimPattern::led_step(const sensors_event_t &sensor_event, Adafruit_BNO055 &bno_sensor) {
  fadeToBlackBy(display_->leds(), display_->max_index(), 10);

  // Get new hues. Overflow is intended.
  uint8_t hue = sensor_event.orientation.x
      + sensor_event.orientation.y
      + sensor_event.orientation.z;
  // TODO: This should be a utility function.

  imu::Vector<3> lin_accel_v
      = bno_sensor.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  float lin_accel = lin_accel_v.x() + lin_accel_v.y() + lin_accel_v.z();

  uint32_t pos = rand() % display_->max_index();
  uint32_t pos_n1 = pos == 0 ? 0 : pos - 1;
  uint32_t pos_p1 = pos == display_->max_index() ? display_->max_index()
                                                 : pos + 1;

  // uint32_t pos = 2;
  display_->leds()[pos].setHSV(hue, 255, lin_accel);
  display_->leds()[pos_n1].setHSV(hue, 255, lin_accel);
  display_->leds()[pos_p1].setHSV(hue, 255, lin_accel);

}
