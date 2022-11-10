#include "initialize.h"
#include "constants.h"
#include "Arduino.h"

void setup()
{
  pinMode(SLED_PIN, OUTPUT);
  initialize::io();
  initialize::emergencyOTA();
  initialize::peripherals();
  initialize::power();
  initialize::debugServer();
}

void loop()
{
  // Serial.print(".");
  digitalWrite(SLED_PIN, HIGH);
  delay(1000);
  digitalWrite(SLED_PIN, LOW);
  delay(1000);
}
