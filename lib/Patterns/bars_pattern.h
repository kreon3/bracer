#ifndef BARS_PATTERN_H_
#define BARS_PATTERN_H_

#include "pattern.h"

#define NUM_BARS 4
#define PIXEL_WIDTH 28
#define PIXEL_HEIGHT 4

class Bar {
 public:
  Bar();
  void reset(uint8_t hue,
        uint8_t saturation,
        uint8_t max_value,
        uint8_t current_value,
        uint8_t change_per_step,
        bool increasing,
        uint8_t row,
        uint8_t col,
        uint8_t taper);

  uint8_t hue_;
  uint8_t saturation_;
  uint8_t max_value_;
  uint8_t current_value_;
  uint8_t change_per_step_;
  bool increasing_;
  uint8_t row_;
  uint8_t col_;
  uint8_t taper_;
};

class BarsPattern : public Pattern {
public:
  BarsPattern(LedDisplay *display, unsigned long step_frequency_ms);
  void start() override;

  void led_step(const sensors_event_t &sensor_event,
                Adafruit_BNO055 &bno_sensor) override;

private:
  void initBar(Bar* bar, uint8_t row);
  void stepBar(Bar* bar);
  void pick_unused_pixel(uint8_t* row, uint8_t* col);
  void setRowColor(uint8_t row, CRGB color);
  void drawBar(Bar* bar);

  Bar bars_[NUM_BARS];
};



#endif // BARS_PATTERN_H_
