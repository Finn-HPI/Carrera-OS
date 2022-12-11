#include "Config.h"

Config initialConfig{
    .full_speed = 125,
    .min_speed = 10,

    .loop_time = 6,

    .acceleration = 255 * 4,
    .deceleration = 255,

    .irl_time = 2000};

Config* config = &initialConfig;
