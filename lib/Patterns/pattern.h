#include <Adafruit_BNO055.h>

class Pattern {
public:
  // Starts the pattern.
  virtual void start() = 0;
  // Update the pattern.
  virtual void update(unsigned long millis,
                      const sensors_event_t &sensor_event) = 0;
};
