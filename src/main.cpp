#include "initialize.h"
#include "constants.h"
#include "Arduino.h"

void setup()
{
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
