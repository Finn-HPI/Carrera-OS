#include "initialize.h"

#include <esp_pm.h>
#include <esp32-hal-cpu.h>

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
  ledcSetup(MOTOR_PWM_CHANNEL, 25000/*Hz*/, /*resolution (bit)*/ 8);
  ledcAttachPin(MOTOR_PIN, MOTOR_PWM_CHANNEL);
  ledcWrite(MOTOR_PWM_CHANNEL, 0);
  delay(10);

  ledcSetup(MOTOR_BRK_PWM_CHANNEL, 25000/*Hz*/, /*resolution (bit)*/ 8);
  ledcAttachPin(MOTOR_BRK, MOTOR_BRK_PWM_CHANNEL);
  ledcWrite(MOTOR_BRK_PWM_CHANNEL, 255);
  delay(10);
  ledcWrite(MOTOR_BRK_PWM_CHANNEL, 0);

  pinMode(IRLED_PIN, OUTPUT);
  digitalWrite(IRLED_PIN, LOW);

  ledcSetup(SLED_PWM_CHANNEL, 25000/*Hz*/, /*resolution (bit)*/ 8);
  ledcAttachPin(SLED_PIN, SLED_PWM_CHANNEL);
  ledcWrite(SLED_PWM_CHANNEL, 0);
  
  pinMode(TRK_PIN, INPUT);
  pinMode(ADC_PIN, INPUT);
}

void initialize::peripherals() {
  // Wire.begin(SDA, SCL);

  // Serial.begin(115200);
  
  // if (!mpu.begin()) {
  //   Serial.println("Sensor init failed");
  //   while (1)
  //     yield();
  // }
  // Serial.println("Found MPU-6050");

  // Wire.setClock(400000);

  // //wait for MPU to not freak out
  // delay(500);
}

void initialize::power() {
   // reduce power consumption
  setCpuFrequencyMhz(160);
  Serial.print("CPU Freq: ");
  Serial.println(getCpuFrequencyMhz());
}

void initialize::debugServer() {  
  server.setup();
  ledcWrite(SLED_PWM_CHANNEL, 255);
}


float vin_energy_arr[2048];
float capacitor_energy_arr[2048];
float diffsStorage[2048];
std::pair<int16_t, int> timingsStorage[4096];

void initialize::storages() {
  // vin_energy.setStorage(vin_energy_arr);
  // capacitor_energy.setStorage(capacitor_energy_arr);
  // diffs.setStorage(diffsStorage);
}

void initialize::strategy() {
  // ::strategy.store(&NullStrategy::instance);
  // ::strategy.load()->initialize();
}
