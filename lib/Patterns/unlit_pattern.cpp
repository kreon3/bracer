
#include "unlit_pattern.h"

void UnlitPattern::start() {
  Pattern::start();
  set_all(0x000000);
}
void UnlitPattern::led_step(const sensors_event_t &sensor_event,
                            Adafruit_BNO055 &bno_sensor) {                              
}
