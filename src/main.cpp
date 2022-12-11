#include "initialize.h"
#include "constants.h"
#include "Arduino.h"
#include "driving.h"

#include "WiFi.h"

void setup()
{
  initialize::io();
  initialize::emergencyOTA();
  initialize::power();
  initialize::carreraServer();
}

void loop()
{
  server.loop();
  driving::tick(config->loop_time);
}
