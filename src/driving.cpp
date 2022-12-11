#include "driving.h"

#include "constants.h"

float target_speed = 0;
float real_speed = 0;

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

void driving::setSpeed(int new_speed) {
    // If the speed get's too low, the motor will block and potentially burn out.
    // Therefore, just completely stop the motor in this case.
    if (new_speed < config->min_speed) {
        new_speed = 0;
    }
    target_speed = new_speed;
}

void driving::tick(float milliseconds) {
    if (target_speed > real_speed) {
        accelerate(255);
        real_speed += config->acceleration * (milliseconds / 1000);
        if (target_speed <= real_speed) {
            real_speed = target_speed;
        }
    } else if (target_speed < real_speed) {
        decelerate(255);
        real_speed -= config->deceleration * (milliseconds / 1000);

        if (real_speed >= target_speed) {
            real_speed = target_speed;
        }
    } else {
        // target_speed and real_speed are equal
        accelerate(target_speed);
    }
}

int driving::getSpeed() {
    return real_speed;
}
