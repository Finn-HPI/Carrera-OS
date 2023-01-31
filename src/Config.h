#pragma once

struct Config {
    int full_speed;
    int min_speed;

    int loop_time;
    int frequency;

    int acceleration;
    int deceleration;

    int boost_duration;
    int boost_power;

    int irl_time;

    bool instant_acceleration;
};

extern struct Config* config;
