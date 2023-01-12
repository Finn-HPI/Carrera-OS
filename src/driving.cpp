#include "driving.h"

float target_speed = 0;
float real_speed = 0;
float boost_activation_time = 0;

void decelerate(uint32_t duty) {
    ledcWrite(MOTOR_PWM_CHANNEL, 0);
    delay(1);
    ledcWrite(MOTOR_BRK_PWM_CHANNEL, duty);
}

void accelerate(uint32_t duty) {
    ledcWrite(MOTOR_BRK_PWM_CHANNEL, 0);
    delay(1);
    ledcWrite(MOTOR_PWM_CHANNEL, duty);
}

void driving::tick(float milliseconds) {
    if (millis() - boost_activation_time < config->boost_duration) {
        accelerate(config->boost_power);
        return;
    }
    if (!config->instant_acceleration) {
        accelerate(target_speed);
        return;
    }
    if (real_speed < target_speed) {
        accelerate(255);
        real_speed += config->acceleration * (milliseconds / 1000);
        if (real_speed >= target_speed) {
            real_speed = target_speed;
        }
    } else if (real_speed > target_speed) {
        decelerate(255);
        real_speed -= config->deceleration * (milliseconds / 1000);
        if (real_speed <= target_speed) {
            real_speed = target_speed;
        }
    } else {  // target_speed and real_speed are equal
        accelerate(target_speed);
    }
}

void driving::boost() {
    boost_activation_time = millis();
    accelerate(config->boost_power);
}

int driving::setSpeed(int new_speed) {
    // If the speed get's too low, the motor will block and potentially burn out.
    // Therefore, just completely stop the motor in this case.
    if (new_speed < config->min_speed) {
        new_speed = 0;
    }
    target_speed = new_speed;
    return target_speed;
}

int driving::getSpeed() {
    return real_speed;
}
