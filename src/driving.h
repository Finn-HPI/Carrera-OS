#pragma once
#include "constants.h"
namespace driving {
int setSpeed(int newSpeed);

void boost();

// getSpeed returns the target_speed
int getSpeed();

void tick(float milliseconds = config->loop_time);
}  // namespace driving