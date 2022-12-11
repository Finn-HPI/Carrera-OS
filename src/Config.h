#pragma once

struct Config {
    int full_speed;
    int min_speed;

    int loop_time;

    int acceleration;
    int deceleration;

    int irl_time;
};

extern struct Config* config;
