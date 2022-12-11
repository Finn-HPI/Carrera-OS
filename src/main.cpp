#include "constants.h"
#include "driving.h"
#include "initialize.h"

void setup() {
    initialize::io();
    initialize::emergencyOTA();
    initialize::power();
    initialize::carreraServer();
}

void loop() {
    server.loop();
    driving::tick(config->loop_time);
}
