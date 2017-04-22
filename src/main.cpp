/**
 * Full wiring test for the bracer project.
 *
 */
#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// TODO:
//   Modify board LED state based on the xor of the two buttons.
//   Read the accelerometer data and write it to serial.
//   Output initialize pattern for the LEDs.
//   Use the accelerometer to color a sequence of the LEDs.

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define BUTTON_A_PIN 4
#define BUTTON_B_PIN 8

// Button Circuitry:
// Arduino 5v <-> Button Terminal Right
// Button Terminal Left <-> Arduino Pin 4/8
// Button Terminal Left <-> 10k Resistor <-> Arduino Ground/General Ground

// BNO055 Circuitry:
// Arduino 5v <-> BNO055 Vin
// Ground <-> BNO055 Ground
// Arduino A4/SDA <-> BNO055 SDA
// Arduino A5/SCL <-> BNO055 SCL

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

void displayBnoSensorDetails(void) {
  sensor_t sensor;
  bno_sensor.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
}

void displayBnoSensorCalStatus(void)
{
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
      /* Get a new sensor event */
    sensors_event_t event;
    bno_sensor.getEvent(&event);

    /* Display the floating point data */
    Serial.print("X: ");
    Serial.print(event.orientation.x, 4);
    Serial.print("\tY: ");
    Serial.print(event.orientation.y, 4);
    Serial.print("\tZ: ");
    Serial.print(event.orientation.z, 4);
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

void updateBnoSensor(unsigned long millis) {

}

void printBnoSensorState() {
  displayBnoSensorOrientation();
  displayBnoSensorCalStatus();
}


////////////////////////////////////////////////////////////////////////////////
// Common Section

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  initButtons();
  initBnoSensor();
  
  Serial.begin(115200);
  Serial.println("Setup complete.");
}

void printSensorState() {
  printButtonState();
  printBnoSensorState();
  Serial.println("");
}

// Perform everything using a timer so constant polling is available.
void update(unsigned long millis) {
  digitalWrite(LED_BUILTIN, millis % 1000 > 500 ? HIGH : LOW);
  updateButtons(millis);
  updateBnoSensor(millis);
  printSensorState();
}

void loop() {
  update(millis());
}
