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

#include "stm32_sdk_hal.h"

extern volatile ADC_TypeDef *stm32_adc_get_regs(int unit_no);

#if defined(STM32F2) || defined(STM32F4) || defined(STM32F7)

// TODO

bool mgos_adc_init(void) {
  return true;
}
#endif  // defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
