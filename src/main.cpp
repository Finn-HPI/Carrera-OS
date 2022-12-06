#include "initialize.h"
#include "constants.h"
#include "Arduino.h"
#include "driving.h"

#include "WiFi.h"
#include <soc/sens_reg.h>
#include <soc/sens_struct.h>
#include <driver/adc.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Start setup...");
  initialize::io();
  initialize::emergencyOTA();
  initialize::peripherals();
  initialize::power();
  initialize::storages();
  initialize::carreraServer();
  Serial.println("Setup done!");

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
}

static int adc1_convert(int channel)
{
    uint16_t adc_value;
    SENS.sar_meas_start1.sar1_en_pad = (1 << channel); //only one channel is selected.
    while (SENS.sar_slave_addr1.meas_status != 0);
    SENS.sar_meas_start1.meas1_start_sar = 0;
    SENS.sar_meas_start1.meas1_start_sar = 1;
    while (SENS.sar_meas_start1.meas1_done_sar == 0);
    adc_value = SENS.sar_meas_start1.meas1_data_sar;
    return adc_value;
}

int adc1_get_raw_no_lock(adc1_channel_t channel)
{
    //start conversion
    return adc1_convert(channel);
}

void loop()
{
  adc1_get_raw(ADC1_CHANNEL_4);
  adc1_get_raw(ADC1_CHANNEL_6);

  while(vin_energy.size() < vin_energy.max_size()) {
        vin_energy.push_back(adc1_get_raw_no_lock(ADC1_CHANNEL_4));
  }
  while(capacitor_energy.size() < capacitor_energy.max_size()) {
      capacitor_energy.push_back(adc1_get_raw_no_lock(ADC1_CHANNEL_6));
  }

  server.loop();
  driving::tick(config->fastLoopTime / 1000);
}
