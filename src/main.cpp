/**
 * Full wiring test for the bracer project.
 *
 */
#include "Arduino.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

// !!!TODO: Need to get clang lookups working

// TODO:
//   Modify board LED state based on the xor of the two buttons.
//   Read the accelerometer data and write it to serial.
//   Output initialize pattern for the LEDs.
//   Use the accelerometer to color a sequence of the LEDs.


#define BUTTON_A_PIN 4
#define BUTTON_B_PIN 8

// Button circuitry:
// Arduino 5v <-> Button Terminal Right
// Button Terminal Left <-> Arduino Pin 4/8
// Button Terminal Left <-> 10k Resistor <-> Arduino Ground/General Ground

int button_a_state = 0;
int button_b_state = 0;
void readButtons() {
  button_a_state = digitalRead(BUTTON_A_PIN);
  button_b_state = digitalRead(BUTTON_B_PIN);
}


void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BUTTON_A_PIN, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);
  Serial.begin(115200);
  Serial.println("Setup complete.");
}

void printSensorState() {
  Serial.print("B[a]: ");
  Serial.print(button_a_state);
  Serial.print(" B[b]: ");
  Serial.print(button_b_state);

  Serial.println("");
}

// Perform everything using a timer so constant polling is available.
void update(unsigned long millis) {
  digitalWrite(LED_BUILTIN, millis % 1000 > 500 ? HIGH : LOW);
  readButtons();
  printSensorState();
}

void loop()
{
  update(millis());
}
