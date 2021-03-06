#ifndef LIB_FIRE_PATTERN_H_
#define LIB_FIRE_PATTERN_H_

#include "pattern.h"
#include <FastLED.h>
#include "sensor_util.h"

class FirePattern : public Pattern {
public:
  FirePattern(
    LedDisplay *display,
    unsigned long step_frequency_ms);
  FirePattern(
    LedDisplay *display,
    unsigned long step_frequency_ms,
    CRGBPalette16 color_palette);
  void start() override;

  void led_step(const sensors_event_t &sensor_event,
                Adafruit_BNO055 &bno_sensor) override;

private:
  byte get_heat(uint8_t row, uint8_t column);
  void set_heat(uint8_t row, uint8_t column, byte value);
  void heat_update(uint8_t intensity, Position position);
  void draw_heat();

  CRGBPalette16 color_palette_;
  byte* heat_;
};

#endif // LIB_FIRE_PATTERN_H_
