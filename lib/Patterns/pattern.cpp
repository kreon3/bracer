#include "pattern.h"
#include <Logging.h>

Pattern::Pattern(LedDisplay *display, unsigned long step_frequency_ms)
    : display_(display), step_frequency_ms_(step_frequency_ms),
      last_led_update_(0) {}

void Pattern::set_all(CRGB color) {
  for (int r = 0; r < display_->height(); ++r) {
    for (int c = 0; c < display_->width(); ++c) {
      display_->set_color(c, r, color);
    }
  }
}

void Pattern::start() {
  last_led_update_ = 0;
}

void Pattern::update(unsigned long millis,
                     const sensors_event_t &sensor_event) {
  size_t led_steps;

  if (last_led_update_ != 0) {
    led_steps = (millis - last_led_update_) / step_frequency_ms_;
  } else {
    // Initial case, one step update.
    led_steps = 1;
  }

  for (size_t i = 0; i < led_steps; ++i) {
    led_step(sensor_event);
  }

  // Update the last_led_update to the base of the step so if the next step
  // comes within the next few milliseconds an update is properly performed.
  last_led_update_ = millis - millis % step_frequency_ms_;
  if (led_steps > 0) {
    if (led_steps > 1) {
      LOG(WARNING, "Slow led update, steps: ");
      LOGLN(WARNING, led_steps);
    }
    // Only update the strip if there are pattern changes.
    LOGLN(DEBUG, "FastLED.show()");
    display_->show();
  }
}
