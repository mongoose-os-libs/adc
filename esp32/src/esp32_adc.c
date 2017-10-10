/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "esp32/esp32_adc.h"

#include <stdbool.h>

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "common/platform.h"

#include "mgos_adc.h"
#include "mgos_sys_config.h"

#define MGOS_ESP32_ADC1_WIDTH ADC_WIDTH_12Bit

static int s_vref = 1100;

struct esp32_adc_channel_info {
  int pin;
  adc1_channel_t ch;
  adc_atten_t atten;
  esp_adc_cal_characteristics_t chars;
};

static struct esp32_adc_channel_info s_chans[8] = {
    {.pin = 36, .ch = ADC1_CHANNEL_0},
    {.pin = 37, .ch = ADC1_CHANNEL_1},
    {.pin = 38, .ch = ADC1_CHANNEL_2},
    {.pin = 39, .ch = ADC1_CHANNEL_3},
    {.pin = 32, .ch = ADC1_CHANNEL_4},
    {.pin = 33, .ch = ADC1_CHANNEL_5},
    {.pin = 34, .ch = ADC1_CHANNEL_6},
    {.pin = 35, .ch = ADC1_CHANNEL_7},
};

static struct esp32_adc_channel_info *esp32_adc_get_channel_info(int pin) {
  for (int i = 0; i < ARRAY_SIZE(s_chans); i++) {
    if (s_chans[i].pin == pin) return &s_chans[i];
  }
  return NULL;
}

static bool esp32_update_channel_settings(struct esp32_adc_channel_info *ci) {
  if (adc1_config_width(MGOS_ESP32_ADC1_WIDTH) != ESP_OK ||
      adc1_config_channel_atten(ci->ch, ci->atten) != ESP_OK) {
    return false;
  }
  esp_adc_cal_get_characteristics(s_vref, ci->atten, MGOS_ESP32_ADC1_WIDTH,
                                  &ci->chars);
  return true;
}

bool mgos_adc_enable(int pin) {
  struct esp32_adc_channel_info *ci = esp32_adc_get_channel_info(pin);
  if (ci == NULL) return false;

  /* TODO(rojer): Allow changing? */
  ci->atten = ADC_ATTEN_11db;

  return esp32_update_channel_settings(ci);
}

int mgos_adc_read(int pin) {
  struct esp32_adc_channel_info *ci = esp32_adc_get_channel_info(pin);
  if (ci == NULL) return false;
  return adc1_get_raw(ci->ch);
}

int mgos_adc_read_voltage(int pin) {
  struct esp32_adc_channel_info *ci = esp32_adc_get_channel_info(pin);
  if (ci == NULL) return false;
  return adc1_to_voltage(ci->ch, &ci->chars);
}

void esp32_adc_set_vref(int vref_mv) {
  s_vref = vref_mv;
}

bool mgos_adc_init(void) {
  if (mgos_sys_config_get_sys_esp32_adc_vref() > 0) {
    esp32_adc_set_vref(mgos_sys_config_get_sys_esp32_adc_vref());
  }
  return true;
}
