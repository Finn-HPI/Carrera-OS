#include "initialize.h"

#include <esp32-hal-cpu.h>
#include <esp_pm.h>

#include "constants.h"

void initialize::emergencyOTA() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    // wait for pullup to pull the input pin high
    delay(100);

    if (!digitalRead(BUTTON_PIN)) {
        server.emergencyOTA();
    }
}

void initialize::io() {
    // set motor to default speed
    ledcSetup(MOTOR_PWM_CHANNEL, 25000 /*Hz*/, /*resolution (bit)*/ 8);
    ledcAttachPin(MOTOR_PIN, MOTOR_PWM_CHANNEL);
    ledcWrite(MOTOR_PWM_CHANNEL, 0);
    delay(10);

    ledcSetup(MOTOR_BRK_PWM_CHANNEL, 25000 /*Hz*/, /*resolution (bit)*/ 8);
    ledcAttachPin(MOTOR_BRK, MOTOR_BRK_PWM_CHANNEL);
    ledcWrite(MOTOR_BRK_PWM_CHANNEL, 255);
    delay(10);
    ledcWrite(MOTOR_BRK_PWM_CHANNEL, 0);

    ledcSetup(SLED_PWM_CHANNEL, 25000 /*Hz*/, /*resolution (bit)*/ 8);
    ledcAttachPin(SLED_PIN, SLED_PWM_CHANNEL);
    ledcWrite(SLED_PWM_CHANNEL, 0);

    init_irled(IRLED_FREQUENCY);

    pinMode(TRK_PIN, INPUT);
    pinMode(ADC_PIN, INPUT);
}

void initialize::init_irled(int frequency) {
    ledcSetup(IRLED_PWM_CHANNEL, frequency, 8);
    ledcAttachPin(IRLED_PIN, IRLED_PWM_CHANNEL);
    ledcWrite(IRLED_PWM_CHANNEL, 0);
}

void initialize::power() {
    // reduce power consumption
    setCpuFrequencyMhz(160);
}

void initialize::carreraServer() {
    server.setup();
    ledcWrite(SLED_PWM_CHANNEL, 255);
}
