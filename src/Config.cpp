#include "Config.h"

Config initialConfig{
    .full_speed = 125,
    .min_speed = 10,

    .loop_time = 6,
    .frequency = 2600, // Car 6

    .acceleration = 255 * 4,
    .deceleration = 255,

    .boost_duration = 100,
    .boost_power = 200,

    .irl_time = 1000,
    
    .instant_acceleration = false};

Config* config = &initialConfig;
