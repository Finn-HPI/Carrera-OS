#include "driving.h"

#include "constants.h"

float targetSpeed = 0;
float realSpeed = 0;

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

void driving::setSpeed(int newSpeed) {
  // If the speed get's too low, the motor will block and potentially burn out.
  // Therefore, just completely stop the motor in this case.
  if(newSpeed < config->minSpeed) {
    newSpeed = 0;
  }

  // ledcWrite(MOTOR_PWM_CHANNEL, newSpeed);
  targetSpeed = newSpeed;
}

void driving::tick(float milliseconds) {
  if (targetSpeed > realSpeed) {
    accelerate(255);
    realSpeed += config->acceleration * (milliseconds / 1000);
    if (targetSpeed <= realSpeed) {
      realSpeed = targetSpeed;
    }
  }
  else if(targetSpeed < realSpeed) {
    decelerate(255);
    realSpeed -= config->deceleration * (milliseconds / 1000);

    if(realSpeed >= targetSpeed) {
      realSpeed = targetSpeed;
    }
  }
  else {
    // targetSpeed and realSpeed are equal
    accelerate(targetSpeed);
  }
}


int driving::getSpeed() {
  return realSpeed;
}
