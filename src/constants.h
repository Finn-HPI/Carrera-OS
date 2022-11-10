#pragma once

#include "DebugServer.h"

using namespace std;

#define BUTTON_PIN 23
#define SLED_PIN 26
#define IRLED_PIN 18


#define ADC_PIN 34
#define TRK_PIN 32

#define INTERRUPT_PIN 13

//I2C interface
#define SDA 21
#define SCL 22

#define MOTOR_PIN 14
#define MOTOR_BRK 12

#define MOTOR_PWM_CHANNEL 0
#define MOTOR_BRK_PWM_CHANNEL 1
#define SLED_PWM_CHANNEL 2

extern DebugServer server;
