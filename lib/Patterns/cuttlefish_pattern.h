#ifndef LIB_CUTTLEFISH_PATTERN_H_
#define LIB_CUTTLEFISH_PATTERN_H_

#include "pattern.h"
#include <FastLED.h>
#include "sensor_util.h"


class CuttleFishPattern : public Pattern {
public:
  CuttleFishPattern(
    LedDisplay *display,
    unsigned long step_frequency_ms);
  void start() override;

  void led_step(const sensors_event_t &sensor_event,
                Adafruit_BNO055 &bno_sensor) override;

 private:
  uint8_t hue_a_;
  uint8_t hue_b_;
  float position_;
  float pattern_length_;

  void draw_row(uint8_t row, float position_offset);

  // class ColorOrigin {
  //  public:
  //   // Color to display.
  //   CHSV color_;
  //   // Position of the brightest point for the ColorOrigin.
  //   float center_position_;
  //   // Pixels to emit forward and backwards.
  //   float emit_distance_;
  //   bool enabled_;
  //   ColorOrigin() :
  //     color_(CHSV()),
  //     center_position_(0),
  //     emit_distance_(0),
  //     enabled_(false) { }
  //   ColorOrigin(CHSV color, float center_position, float emit_distance) :
  //     color_(color),
  //     center_position_(center_position),
  //     emit_distance_(emit_distance),
  //     enabled_(false) { }
  // };
  // ColorOrigin color_origins_[8];

  // CRGBPalette16 color_palette_;
};

#endif // LIB_CUTTLEFISH_PATTERN_H_
