/*
 * Copyright (c) 2019 Deomid "rojer" Ryabkov
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos_adc.h"

#include "mgos.h"

#include "stm32_sdk_hal.h"

static int s_vref_mv = -1;

static ADC_TypeDef *const s_regs[] = {
    NULL,
#ifdef ADC1
    ADC1,
#else
    NULL,
#endif

#ifdef ADC2
    ADC2,
#else
    NULL,
#endif

#ifdef ADC3
    ADC3,
#else
    NULL,
#endif
};

// Vbat is connected to ADC through a divider.
// The ratio is different for different device families.
#if defined(STM32L4)
#define STM32_ADC_VBAT_DIV 3
#endif
#if defined(STM32F2)
#define STM32_ADC_VBAT_DIV 2
#endif
#if defined(STM32F4) || defined(STM32F7)
#define STM32_ADC_VBAT_DIV 4
#endif
#ifndef STM32_ADC_VBAT_DIV
#error STM32_ADC_VBAT_DIV not defined for this device
#endif

volatile ADC_TypeDef *stm32_adc_get_regs(int unit_no) {
  if (unit_no >= (int) ARRAY_SIZE(s_regs)) return NULL;
  return s_regs[unit_no];
}

bool mgos_adc_enable(int pin) {
  const struct stm32_adc_input_def *ind = stm32_adc_get_input_def_by_pin(pin);
  if (ind == NULL) return false;
  if ((ind->adc_m & STM32_ADC1_M) == 0) return false;
  struct stm32_adc_config cfg = {.resolution = 12};
  if (!stm32_adc_configure(1, &cfg)) return false;
  return stm32_adc_enable_input(1, ind->input_no);
}

int mgos_adc_read(int pin) {
  const struct stm32_adc_input_def *ind = stm32_adc_get_input_def_by_pin(pin);
  if (ind == NULL) return -1;
  if ((ind->adc_m & STM32_ADC1_M) == 0) return false;
  return stm32_adc_read(1, ind->input_no);
}

int mgos_adc_read_voltage(int pin) {
  int mval = mgos_adc_read(pin);
  if (mval < 0) return mval;
  if (pin == STM32_ADC_PIN_VBAT) mval *= STM32_ADC_VBAT_DIV;
  return (s_vref_mv * mval) / 4095;
}

int stm32_adc_get_vref(void) {
  return s_vref_mv;
}

void stm32_adc_set_vref(int vref_mv) {
  s_vref_mv = vref_mv;
}
