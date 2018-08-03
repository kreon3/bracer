#ifndef SPARKLE_PATTERN_H_
#define SPARKLE_PATTERN_H_

#include "pattern.h"

#define NUM_SPARKLES 30
#define SPARKLE_WIDTH 28
#define SPARKLE_HEIGHT 4

class SparklePattern : public Pattern {
public:
  SparklePattern(LedDisplay *display, unsigned long step_frequency_ms);
  void start() override;

  void led_step(const sensors_event_t &sensor_event,
                Adafruit_BNO055 &bno_sensor) override;

private:
  struct Sparkle {
    Sparkle() { }
    void reset(uint8_t hue,
          uint8_t saturation,
          uint8_t max_value,
          uint8_t current_value,
          uint8_t change_per_step,
          bool increasing,
          uint8_t row,
          uint8_t col);

    uint8_t hue_;
    uint8_t saturation_;
    uint8_t max_value_;
    uint8_t current_value_;
    uint8_t change_per_step_;
    bool increasing_;
    uint8_t row_;
    uint8_t col_;
  };

  void initSparkle(Sparkle* sparkle);
  void stepSparkle(Sparkle* sparkle);
  void pick_unused_pixel(uint8_t* row, uint8_t* col);

  Sparkle sparkles_[NUM_SPARKLES];
  bool active_pixel_map_[SPARKLE_HEIGHT][SPARKLE_WIDTH];
};

#endif // SPARKLE_PATTERN_H_
