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

#include "stm32_adc.h"

#include "mgos.h"

// clang-format off

#ifdef STM32F205xx
static const struct stm32_adc_input_def s_stm32_adc_inputs[] = {
  // TODO
  { .pin = 0,                     .input_no = 0, .adc_m = 0 },
};
#define STM32_INPUTS_DEFINED
#endif

#ifdef STM32F746xx
static const struct stm32_adc_input_def s_stm32_adc_inputs[] = {
  { .pin = STM32_GPIO('A', 0),    .input_no =  0, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('A', 1),    .input_no =  1, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('A', 2),    .input_no =  2, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('A', 3),    .input_no =  3, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('A', 4),    .input_no =  4, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 5),    .input_no =  5, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 6),    .input_no =  6, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 7),    .input_no =  7, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('B', 0),    .input_no =  8, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('B', 1),    .input_no =  9, .adc_m = STM32_ADC1_M | STM32_ADC2_M },

  { .pin = STM32_GPIO('C', 0),    .input_no = 10, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('C', 1),    .input_no = 11, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('C', 2),    .input_no = 12, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('C', 3),    .input_no = 13, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },

  { .pin = STM32_GPIO('F', 4),    .input_no = 14, .adc_m = STM32_ADC3_M },
  { .pin = STM32_GPIO('F', 5),    .input_no = 15, .adc_m = STM32_ADC3_M },

  { .pin = 0,                     .input_no = 16, .adc_m = 0 },

  { .pin = STM32_ADC_PIN_VREFINT, .input_no = 17, .adc_m = STM32_ADC1_M },
  { .pin = STM32_ADC_PIN_TEMP,    .input_no = 18, .adc_m = STM32_ADC1_M },  // T sensor and Vbat share the same channel.
  { .pin = STM32_ADC_PIN_VBAT,    .input_no = 18, .adc_m = STM32_ADC1_M },
};
#define STM32_INPUTS_DEFINED
#endif

#ifdef STM32L475xx
static const struct stm32_adc_input_def s_stm32_adc_inputs[] = {
  { .pin = STM32_ADC_PIN_VREFINT, .input_no =  0, .adc_m = STM32_ADC1_M },
  { .pin = STM32_GPIO('C', 0),    .input_no =  1, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('C', 1),    .input_no =  2, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('C', 2),    .input_no =  3, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('C', 3),    .input_no =  4, .adc_m = STM32_ADC1_M | STM32_ADC2_M | STM32_ADC3_M },
  { .pin = STM32_GPIO('A', 0),    .input_no =  5, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 1),    .input_no =  6, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 2),    .input_no =  7, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 3),    .input_no =  8, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 4),    .input_no =  9, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 5),    .input_no = 10, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 6),    .input_no = 11, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('A', 7),    .input_no = 12, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('C', 4),    .input_no = 13, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('C', 5),    .input_no = 14, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('B', 0),    .input_no = 15, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_GPIO('B', 1),    .input_no = 16, .adc_m = STM32_ADC1_M | STM32_ADC2_M },
  { .pin = STM32_ADC_PIN_TEMP,    .input_no = 17, .adc_m = STM32_ADC1_M | STM32_ADC3_M },
  { .pin = STM32_ADC_PIN_VBAT,    .input_no = 18, .adc_m = STM32_ADC1_M | STM32_ADC3_M },
};
#define STM32_INPUTS_DEFINED
#endif

#ifndef STM32_INPUTS_DEFINED
#error STM32 pin to ADC input mapping is not defined for this device
#endif

const struct stm32_adc_input_def *stm32_adc_get_input_def(int input_no) {
  if (input_no >= (int) ARRAY_SIZE(s_stm32_adc_inputs)) return NULL;
  return &s_stm32_adc_inputs[input_no];
}

const struct stm32_adc_input_def *stm32_adc_get_input_def_by_pin(int pin) {
  for (int i = 0; i < (int) ARRAY_SIZE(s_stm32_adc_inputs); i++) {
    if (s_stm32_adc_inputs[i].pin == pin) return &s_stm32_adc_inputs[i];
  }
  return NULL;
}
