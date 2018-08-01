/**
 * Full wiring test for the bracer project.
 * Chip is stated to be the Atmega328P Chip
 */
#include "Arduino.h"

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <FastLED.h>
#include <Logging.h>
#include <Patterns.h>
#include <Wire.h>
#include <avr/pgmspace.h>

// Including this for platformio dependencies of unused
// deep libraries.
#include <SPI.h>
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
#include <utility/imumaths.h>

// TODO:
//   Modify board LED state based on the xor of the two buttons.
//   Output initialize pattern for the LEDs.
//   Use the accelerometer to color a sequence of the LEDs.

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define BUTTON_A_PIN 4
#define BUTTON_B_PIN 8

#define LED_STRIP_DATA_PIN 9
#define LED_STRIP_CLOCK_PIN 10

// Button Circuitry:
// Arduino 5v <-> Button Terminal Right
// Button Terminal Left <-> Arduino Pin 4/8
// Button Terminal Left <-> 10k Resistor <-> Arduino Ground/General Ground

// BNO055 Circuitry:
// Arduino 5v <-> BNO055 Vin
// Ground <-> BNO055 Ground
// Arduino A4/SDA <-> BNO055 SDA
// Arduino A5/SCL <-> BNO055 SCL

// LED Circuitry:
// Arduino D9 <-> LED Data
// Arduino D10<-> LED Clock

// Patterns modified by buttons and LED sections.
Patterns *patterns_ = nullptr;

////////////////////////////////////////////////////////////////////////////////
// Buttons
int button_a_state = 0;
int button_b_state = 0;

#define BUTTON_ON 1
#define BUTTON_OFF 0

#define BUTTON_BRIGHTNESS_DELAY_MS 400
#define BRIGHTENING_FACTOR 2
#define INITIAL_BRIGHTNESS 64

void initButtons() {
  pinMode(BUTTON_A_PIN, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);
}

int last_button_b_state = 0;
unsigned long last_button_b_update = 0;
bool brightness_raised_while_button_on = false;

void raiseBrightness() {
  uint8_t brightness = FastLED.getBrightness();
  uint16_t new_brightness16 = (uint16_t)brightness * BRIGHTENING_FACTOR;
  uint8_t new_brightness = new_brightness16 < 255 ? new_brightness16 : 255;
  // Always raise for fractional brighness.
  if (new_brightness == brightness && new_brightness != 255) {
    ++new_brightness;
  }
  LOGF(INFO, "Raising brightness to: ");
  LOGLN(INFO, new_brightness);
  FastLED.setBrightness(new_brightness);
}

void lowerBrightness() {
  uint8_t brightness = FastLED.getBrightness();
  uint8_t new_brightness = brightness / BRIGHTENING_FACTOR;
  if (new_brightness == 0) {
    new_brightness = 1;
  }
  LOGF(INFO, "Lowering brightness to: ");
  LOGLN(INFO, new_brightness);
  FastLED.setBrightness(new_brightness);
}

void updateLedBrightnessFromButton(int button_b_state, unsigned long millis) {
  if (button_b_state == BUTTON_ON) {
    if (last_button_b_state == BUTTON_ON) {
      // On-to-on hold.
      if (millis - last_button_b_update > BUTTON_BRIGHTNESS_DELAY_MS) {
        LOGLNF(INFO, "Button - on hold exceeded theshold");
        lowerBrightness();
        // Count the last update as the last threshold on time.
        // last_button_b_update =
        //     millis / BUTTON_BRIGHTNESS_DELAY_MS * BUTTON_BRIGHTNESS_DELAY_MS;
        last_button_b_update = millis;
        last_button_b_state = BUTTON_ON;
        brightness_raised_while_button_on = true;
      } // Else: not long enough to trigger an update.
    } else {
      LOGLNF(INFO, "Button - Off-to-on");
      LOG(INFO, millis);
      // Off-to-on transition.
      last_button_b_state = BUTTON_ON;
      last_button_b_update = millis;
    }
  } else {
    // button off.
    if (last_button_b_state == BUTTON_ON) {
      LOGLNF(INFO, "Button On-to-off");
      LOG(INFO, millis);
      // On-to-off transition.
      if (!brightness_raised_while_button_on) {
        // Quick click on and off.
        raiseBrightness();
      }
    }
    last_button_b_state = BUTTON_OFF;
    last_button_b_update = millis;
    brightness_raised_while_button_on = false;
  }
}

int last_button_a_state_ = BUTTON_OFF;
void changePatternFromButton(int button_a_state, unsigned long millis) {
  if (button_a_state == BUTTON_ON && last_button_a_state_ == BUTTON_OFF) {
    LOGLNF(INFO, "Button A pressed");
    if (patterns_ != nullptr) {
      patterns_->next_pattern();
    }
  }
  last_button_a_state_ = button_a_state;
}

void updateButtons(unsigned long millis) {
  button_a_state = digitalRead(BUTTON_A_PIN);
  button_b_state = digitalRead(BUTTON_B_PIN);
  changePatternFromButton(button_a_state, millis);
  updateLedBrightnessFromButton(button_b_state, millis);
}

void printButtonState() {
  LOGF(DEBUG, "B[a]: ");
  LOG(DEBUG, button_a_state);
  LOGF(DEBUG, " B[b]: ");
  LOG(DEBUG, button_b_state);
}

////////////////////////////////////////////////////////////////////////////////
// BNO055 Sensor
Adafruit_BNO055 bno_sensor = Adafruit_BNO055(55);
sensors_event_t bno_event;

void displayBnoSensorDetails(void) {
  sensor_t sensor;
  bno_sensor.getSensor(&sensor);
  LOGLNF(DEBUG, "------------------------------------");
  LOGF(DEBUG, "Sensor:       ");
  LOGLN(DEBUG, sensor.name);
  LOGF(DEBUG, "Driver Ver:   ");
  LOGLN(DEBUG, sensor.version);
  LOGF(DEBUG, "Unique ID:    ");
  LOGLN(DEBUG, sensor.sensor_id);
  LOGF(DEBUG, "Max Value:    ");
  LOG(DEBUG, sensor.max_value);
  LOGLNF(DEBUG, " xxx");
  LOGF(DEBUG, "Min Value:    ");
  LOG(DEBUG, sensor.min_value);
  LOGLNF(DEBUG, " xxx");
  LOGF(DEBUG, "Resolution:   ");
  LOG(DEBUG, sensor.resolution);
  LOGLNF(DEBUG, " xxx");
  LOGLNF(DEBUG, "------------------------------------");
  LOGLNF(DEBUG, "");
}

void displayBnoSensorCalStatus(void) {
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno_sensor.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  LOGF(DEBUG, "\t");
  if (!system) {
    LOGF(DEBUG, "! ");
  }

  /* Display the individual values */
  LOGF(DEBUG, "Sys:");
  LOG_SPEC(DEBUG, system, DEC);
  LOGF(DEBUG, " G:");
  LOG_SPEC(DEBUG, gyro, DEC);
  LOGF(DEBUG, " A:");
  LOG_SPEC(DEBUG, accel, DEC);
  LOGF(DEBUG, " M:");
  LOG_SPEC(DEBUG, mag, DEC);
}

void displayBnoSensorOrientation() {
  /* Display the floating point data */
  LOGF(DEBUG, "X: ");
  LOG_SPEC(DEBUG, bno_event.orientation.x, 4);
  LOGF(DEBUG, "\tY: ");
  LOG_SPEC(DEBUG, bno_event.orientation.y, 4);
  LOGF(DEBUG, "\tZ: ");
  LOG_SPEC(DEBUG, bno_event.orientation.z, 4);
}

void initBnoSensor() {
  while (!bno_sensor.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    LOGF(ERROR, "No BNO055 detected ... Check your wiring or I2C ADDR!");
  }
  // The external crystal is stated to give better accuracy.
  bno_sensor.setExtCrystalUse(true);

  displayBnoSensorDetails();
}

void updateBnoSensor(unsigned long millis) { bno_sensor.getEvent(&bno_event); }

void printBnoSensorState() {
  displayBnoSensorOrientation();
  displayBnoSensorCalStatus();
}

////////////////////////////////////////////////////////////////////////////////
// LED Strip Section

// 4 strips of 28 pixels.
#define LED_WIDTH 28
#define LED_HEIGHT 4
#define NUM_PIXELS (LED_WIDTH * LED_HEIGHT)

CRGB leds[NUM_PIXELS];

CRGB color = 0x000000;

void updateLeds(unsigned long millis) {
  patterns_->update(millis, bno_event, bno_sensor);
}

void initLeds() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  LOGLNF(DEBUG, "addLeds");
  // Max power is expected to be 6400 mA. Capping at 4000 mW appears to stop
  // lower power artifacts.
  FastLED.setMaxPowerInMilliWatts(4000);
  FastLED.setBrightness(INITIAL_BRIGHTNESS);
  FastLED.addLeds<DOTSTAR, LED_STRIP_DATA_PIN, LED_STRIP_CLOCK_PIN, BGR>(
      leds, NUM_PIXELS);
  // Turn all LEDs off ASAP
  LOGLNF(DEBUG, "fill_solid");
  fill_solid(leds, NUM_PIXELS, CRGB(0, 0, 0));
  LOGLNF(DEBUG, "first show");
  FastLED.show();
  LOGLNF(DEBUG, "first show done");

  patterns_ = new Patterns(leds, LED_WIDTH, LED_HEIGHT);
}

void printLedState() {
  LOGF(DEBUG, "Color: ");
  LOG_SPEC(DEBUG, color.r, HEX);
  LOG_SPEC(DEBUG, color.g, HEX);
  LOG_SPEC(DEBUG, color.b, HEX);
}

////////////////////////////////////////////////////////////////////////////////
// Memory Tracking

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

size_t freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

void warnLowMemory() {
  size_t free_mem = freeMemory();

  if (free_mem < 50) {
    LOGF(ERROR, "Very low mem: ");
    LOGLN(ERROR, free_mem);
  } else if (free_mem < 200) {
    LOGF(WARNING, "Low mem: ");
    LOGLN(WARNING, free_mem);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Common Section

void setup() {
  Serial.begin(115200);
  LOGLNF(INFO, "Setup started.");
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  initLeds();
  initButtons();
  initBnoSensor();

  LOGLNF(INFO, "Setup complete.");
  LOGF(INFO, "Memory: ");
  LOGLN(INFO, freeMemory());
}

void printSensorState() {
  printButtonState();
  LOGF(DEBUG, "\t");
  printLedState();
  LOGF(DEBUG, "\t");
  printBnoSensorState();
  LOGLNF(DEBUG, "");
}


// Perform everything using a timer so constant polling is available.
void update(unsigned long millis) {
  digitalWrite(LED_BUILTIN, millis % 1000 > 500 ? HIGH : LOW);
  updateButtons(millis);
  updateBnoSensor(millis);
  updateLeds(millis);

  // Always warn on low memory as this leads to memory corruption of the stack!
  warnLowMemory();
  printSensorState();
}


void loop() { update(millis()); }
