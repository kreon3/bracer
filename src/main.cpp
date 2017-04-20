/**
 * Full wiring test for the bracer project.
 *
 */
#include "Arduino.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

// TODO:
//   Modify board LED state based on the xor of the two buttons.
//   Read the accelerometer data and write it to serial.
//   Output initialize pattern for the LEDs.
//   Use the accelerometer to color a sequence of the LEDs.

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Setup complete.");
}

size_t iteration = 0;

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  Serial.print("I: ");
  Serial.println(iteration);
  ++iteration;
}
