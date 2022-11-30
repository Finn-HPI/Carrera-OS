#include "initialize.h"
#include "constants.h"
#include "Arduino.h"
#include "driving.h"

#include "WiFi.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Start setup...");
  initialize::io();
  initialize::emergencyOTA();
  initialize::peripherals();
  initialize::power();
  initialize::debugServer();
  Serial.println("Setup done!");
}

void loop()
{
  driving::tick(config->fastLoopTime / 1000);
}
