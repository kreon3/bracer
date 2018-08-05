#include "fire_pattern.h"

#include <Adafruit_BNO055.h>
#include <FastLED.h>
#include <Logging.h>
#include "sensor_util.h"

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
  uint8_t intensity = get_linear_intensity(bno_sensor);
  Position pos = get_position(bno_sensor);
  heat_update(intensity, pos);
  draw_heat();

}

#define COOLING  240
#define SPARKING 120
void FirePattern::heat_update(uint8_t intensity, Position position) {
  for (size_t row = 0; row < display_->rows(); ++row) {
    uint8_t cols = display_->width();
    // Cool cells.
    for( int i = 0; i < cols ; ++i) {
      set_heat(row, i,
        qsub8(get_heat(row, i), random8(0, ((COOLING * 10) / cols) + 2)));
    }


    // Move heat.
    if (position == Position::UP) {
      for (size_t i = cols - 1; i >= 2; --i) {
        // TODO: Update this diffusion so it is more relative to adjacent cells.
        set_heat(row, i, (get_heat(row, i-1) + 2 * get_heat(row, i-2) / 3));
      }
    } else if (position == Position::DOWN) {
      for (size_t i = 0; i <= cols - 2; ++i) {
        // TODO: Update this diffusion so it is more relative to adjacent cells.
        set_heat(row, i, (get_heat(row, i+1) + 2 * get_heat(row, i+2) / 3));
      }
    } else {
      byte last_heat = get_heat(row, 0);
      set_heat(row, 0, get_heat(row, 0) + get_heat(row, 1) / 3);
      for (size_t i = 1; i <= cols - 1; ++i) {
        byte cur_heat = get_heat(row, i) + (last_heat + get_heat(row, i+1)) / 3;
        last_heat = get_heat(row, i);
        set_heat(row, i, cur_heat);
      }
      set_heat(row, cols-1, get_heat(row, cols-1) + last_heat / 3);
    }

    // New sparks.
    uint32_t spark_value = random(210 + intensity);
    if (spark_value > 200) {
      uint8_t spark_col;
      if (position == Position::UP) {
        spark_col = random(0, 3);
      } else if (position == Position::DOWN) {
        spark_col = random(display_->cols() - 3, display_->cols());
      } else {
        spark_col = random(0, display_->cols());
      }
      set_heat(row, spark_col, random8(140, 255));
    }
  }
}

void FirePattern::draw_heat() {
  for (size_t row = 0; row < display_->rows(); ++row) {
    for (size_t col = 0; col < display_->cols(); ++col) {
      byte colorindex = scale8(get_heat(row, col), 240);
      CRGB color = ColorFromPalette( color_palette_, colorindex);
      display_->set_color(col, row, color);
    }
  }
}

byte FirePattern::get_heat(uint8_t row, uint8_t column) {
  return heat_[row * display_->width() + column];
}
void FirePattern::set_heat(uint8_t row, uint8_t column, byte value) {
  heat_[row * display_->width() + column] = value;
}
