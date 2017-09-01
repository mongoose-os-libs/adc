/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "esp_adc.h"

#include <stdlib.h>

#ifdef RTOS_SDK
#include <esp_system.h>
#else
#include <user_interface.h>
#endif

bool mgos_adc_enable(int pin) {
  return pin == 0;
}

int mgos_adc_read(int pin) {
  return pin == 0 ? 0xFFFF & system_adc_read() : -1;
}

static int s_adc_at_boot = 0;

int esp_adc_value_at_boot(void) {
  return s_adc_at_boot;
}

void esp_adc_init(void) {
  s_adc_at_boot = mgos_adc_read(0);
}

bool mgos_adc_init(void) {
  srand(rand() ^ s_adc_at_boot);
  return true;
}
