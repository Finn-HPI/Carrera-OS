#pragma once
#include "constants.h"
namespace driving {
void setSpeed(int newSpeed);

// getSpeed is a guess of the real speed we're currently driving
int getSpeed();

void tick(float milliseconds = config->loop_time);
}  // namespace driving