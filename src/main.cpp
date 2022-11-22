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
  server.loop();
}
