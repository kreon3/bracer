/**
 * Full wiring test for the bracer project.
 * Chip is stated to be the Atmega328P Chip
 */
#include "Arduino.h"

#include <Adafruit_BNO055.h>
// #include <Adafruit_DotStar.h>
#include <Adafruit_Sensor.h>
#include <FastLED.h>
#include <Wire.h>

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

////////////////////////////////////////////////////////////////////////////////
// Buttons
int button_a_state = 0;
int button_b_state = 0;

void initButtons() {
  pinMode(BUTTON_A_PIN, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);
}

void updateButtons(unsigned long millis) {
  button_a_state = digitalRead(BUTTON_A_PIN);
  button_b_state = digitalRead(BUTTON_B_PIN);
}

void printButtonState() {
  Serial.print("B[a]: ");
  Serial.print(button_a_state);
  Serial.print(" B[b]: ");
  Serial.print(button_b_state);
}

////////////////////////////////////////////////////////////////////////////////
// BNO055 Sensor
Adafruit_BNO055 bno_sensor = Adafruit_BNO055(55);
sensors_event_t bno_event;

void displayBnoSensorDetails(void) {
  sensor_t sensor;
  bno_sensor.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" xxx");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" xxx");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
}

void displayBnoSensorCalStatus(void) {
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno_sensor.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system) {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
}

void displayBnoSensorOrientation() {
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(bno_event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(bno_event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(bno_event.orientation.z, 4);
}

void initBnoSensor() {
  while (!bno_sensor.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("No BNO055 detected ... Check your wiring or I2C ADDR!");
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
#define NUM_PIXELS (28 * 4)

CRGB leds[NUM_PIXELS];

// Adafruit_DotStar led_strip = Adafruit_DotStar(NUM_PIXELS, LED_STRIP_DATA_PIN,
//                                               LED_STRIP_CLOCK_PIN,
//                                               DOTSTAR_BRG);

int head = 10, tail = 0; // Index of first 'on' and 'off' pixels
CRGB color = 0xFF0000;   // 'On' color (starts red)
unsigned long last_led_update = 0;
#define LED_FREQUENCY_MS 20;

void colorFromSensor(CRGB *color) {
  color->r = ((int32_t)bno_event.orientation.x % 255);
  color->g = ((int32_t)bno_event.orientation.y % 255);
  color->b = ((int32_t)bno_event.orientation.z % 255);
}

// Perform the loop every 20ms.
void ledStep() {
  Serial.println("ledStep start");
  colorFromSensor(&color);
  leds[head] = color;    // 'On' pixel at head
  leds[tail] = 0x000000; // 'Off' pixel at tail

  if (++head >= NUM_PIXELS) { // Increment head index.  Off end of strip?
    head = 0;                 //  Yes, reset head index to start
  }
  if (++tail >= NUM_PIXELS) {
    tail = 0; // Increment, reset tail index
  }
  Serial.println("ledStep done");
}

void updateLeds(unsigned long millis) {
  size_t led_steps;

  if (last_led_update != 0) {
    led_steps = (millis - last_led_update) / LED_FREQUENCY_MS;
  } else {
    // Initial case, one step update.
    led_steps = 1;
  }

  for (size_t i = 0; i < led_steps; ++i) {
    ledStep();
  }

  // Update the last_led_update to the base of the step so if the next step
  // comes within the next few milliseconds an update is properly performed.
  last_led_update = millis - millis % LED_FREQUENCY_MS;
  if (led_steps > 0) {
    // Only update the strip if there are pattern changes.
    Serial.println("FastLED.show()");
    FastLED.show();
  }
}

void initLeds() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  Serial.println("addLeds");
  FastLED.addLeds<DOTSTAR, LED_STRIP_DATA_PIN, LED_STRIP_CLOCK_PIN>(leds,
                                                                    NUM_PIXELS);
  // Turn all LEDs off ASAP
  Serial.println("fill_solid");
  fill_solid(leds, NUM_PIXELS, CRGB(100, 0, 0));
  Serial.println("first show");
  FastLED.show();
  Serial.println("first show done");
}

void printLedState() {
  Serial.print("Color: ");
  Serial.print(color.r, HEX);
  Serial.print(color.g, HEX);
  Serial.print(color.b, HEX);
}

////////////////////////////////////////////////////////////////////////////////
// Common Section

void setup() {
  // initialize LED digital pin as an output.
  Serial.begin(115200);
  Serial.println("Setup started.");

  pinMode(LED_BUILTIN, OUTPUT);

  initLeds();
  initButtons();
  initBnoSensor();

  Serial.println("Setup complete.");
}

void printSensorState() {
  printButtonState();
  Serial.print("\t");
  printLedState();
  Serial.print("\t");
  printBnoSensorState();
  Serial.println("");
}

// Perform everything using a timer so constant polling is available.
void update(unsigned long millis) {
  digitalWrite(LED_BUILTIN, millis % 1000 > 500 ? HIGH : LOW);
  updateButtons(millis);
  updateBnoSensor(millis);
  updateLeds(millis);
  printSensorState();
}

void loop() { update(millis()); }
