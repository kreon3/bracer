
#include "fire_pattern.h"
#include <Adafruit_BNO055.h>
#include <FastLED.h>
#include <Logging.h>

FirePattern::FirePattern(LedDisplay *display, unsigned long step_frequency_ms)
  : FirePattern(display, step_frequency_ms, HeatColors_p) {
}

FirePattern::FirePattern(
  LedDisplay *display,
  unsigned long step_frequency_ms,
  CRGBPalette16 color_palette)
  : Pattern(display, step_frequency_ms),
    color_palette_(color_palette) {
    heat_ = static_cast<byte*>(
        malloc(display_->height() * display_->width()));
}

void FirePattern::start() {
  memset(heat_, 0, display_->height() * display_->width());
  Pattern::start();
  set_all(0x000000);
}

void FirePattern::led_step(const sensors_event_t &sensor_event,
                            Adafruit_BNO055 &bno_sensor) {

  imu::Vector<3> lin_accel_v
      = bno_sensor.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  // TODO: Refactor this out so other patterns can use this calculation.
  // Expected values for intensity should be 0-200. Expecting max linear
  // acceleration to be 4m/s.
  uint8_t intensity = lin_accel_v.magnitude() * (200 / 4);
  LOGF(DEBUG, "Intensity: ");
  LOGLN(DEBUG, intensity);

  for (uint8_t i = 0; i < display_->height(); ++i) {
    step_row(i, intensity);
  }
}

#define COOLING  240
#define SPARKING 120
void FirePattern::step_row(uint8_t row, uint8_t intensity) {
  uint8_t cols = display_->width();
  // Cool cells.
  for( int i = 0; i < cols ; ++i) {
    set_heat(row, i,
      qsub8(get_heat(row, i), random8(0, ((COOLING * 10) / cols) + 2)));
  }

  // Heat goes up.
  for( int i = cols - 1; i >= 2; --i) {
    // TODO: Update this diffusion so it is more relative to adjacent cells.
    set_heat(row, i, (get_heat(row, i-1) + 2 * get_heat(row, i-2) / 3));
  }

  // New sparks.
  uint32_t spark_value = random(205 + intensity);
  if (spark_value > 200) {
    uint8_t spark_col = random8(3);
    set_heat(row, spark_col, random8(140, 255));
  }

  // Map from heat cells to LED colors.
   for( int i = 0; i < cols; ++i) {
     // Scale the heat value from 0-255 down to 0-240
     // for best results with color palettes.
     byte colorindex = scale8( get_heat(row, i), 240);
     CRGB color = ColorFromPalette( color_palette_, colorindex);
     display_->set_color(i, row, color);
     // int pixelnumber;
     // if( gReverseDirection ) {
     //   pixelnumber = (NUM_LEDS-1) - j;
     // } else {
     //   pixelnumber = j;
     // }
     // leds[pixelnumber] = color;
   }
}

byte FirePattern::get_heat(uint8_t row, uint8_t column) {
  return heat_[row * display_->width() + column];
}
void FirePattern::set_heat(uint8_t row, uint8_t column, byte value) {
  heat_[row * display_->width() + column] = value;
}
