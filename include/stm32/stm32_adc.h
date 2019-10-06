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

#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Value of the internal voltage reference.
#ifndef STM32_ADC_VREFINT_VALUE_MV
#define STM32_ADC_VREFINT_VALUE_MV 1212
#endif

struct stm32_adc_config {
  // Resolution, in bits; 6, 8, 10 or 12; default is 12.
  uint8_t resolution;
};

// Configures and enabled the ADC.
bool stm32_adc_configure(int unit_no, const struct stm32_adc_config *cfg);

// Enable particular ADC input.
bool stm32_adc_enable_input(int unit_no, int input_no);

// Performs single conversion of a particular input (0-18)
int stm32_adc_read(int unit_no, int input_no);

/* Read the internal temperature sensor.
 * Returns temperature in degrees Celsius. */
int stm32_adc_read_temp_c(void);

// Returns reference value used for mgos_adc_read_voltage calculation.
int stm32_adc_get_vref(void);

// Set reference voltage value, used in mgos_adc_read_voltage calculation.
void stm32_adc_set_vref(int vref_mv);

// Sets vref by measuring Vrefint.
// Returns calculated value or -1 in case of error.
int stm32_adc_set_vref_from_vrefint(void);

// ADC "pseudo-pins" for internal sources.
#define STM32_ADC_PIN_VREFINT 0xff0000  // Internal voltage reference
#define STM32_ADC_PIN_TEMP 0xff0001     // Package temperature sensor
#define STM32_ADC_PIN_VBAT 0xff0002     // Battery voltage rail

#define STM32_ADC1_M (1 << 1)
#define STM32_ADC2_M (1 << 2)
#define STM32_ADC3_M (1 << 3)

struct stm32_adc_input_def {
  int pin;           // GPIO pin (port, number).
  uint8_t input_no;  // Input number.
  uint8_t
      adc_m;  // Which ADC units have it enabled, combination of STM32_ADCx_M.
};

const struct stm32_adc_input_def *stm32_adc_get_input_def(int input_no);
const struct stm32_adc_input_def *stm32_adc_get_input_def_by_pin(int pin);

#ifdef __cplusplus
}
#endif
