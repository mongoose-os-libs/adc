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

extern volatile ADC_TypeDef *stm32_adc_get_regs(int unit_no);

#if defined(STM32L4)
bool stm32_adc_configure(int unit_no, const struct stm32_adc_config *cfg) {
  volatile ADC_TypeDef *regs = stm32_adc_get_regs(unit_no);
  if (regs == NULL) return false;
  uint32_t res_val = 0;
  switch (cfg->resolution) {
    case 6:
      res_val = 3;
      break;
    case 8:
      res_val = 2;
      break;
    case 10:
      res_val = 1;
      break;
    case 0:
    case 12:
      res_val = 0;
      break;
    default:
      return false;
  }
  // Power-up sequence, as described in RM0351 18.4.6
  if (regs->CR & ADC_CR_DEEPPWD) {
    CLEAR_BIT(regs->CR, ADC_CR_DEEPPWD);
  }
  if (!(regs->CR & ADC_CR_ADVREGEN)) {
    SET_BIT(regs->CR, ADC_CR_ADVREGEN);
    mgos_usleep(20);
  }
  if (!(regs->CR & ADC_CR_ADEN) &&
      (regs->CALFACT & ADC_CALFACT_CALFACT_S) == 0) {
    // Perform single-ended input calibration.
    CLEAR_BIT(regs->CR, ADC_CR_ADCALDIF);
    SET_BIT(regs->CR, ADC_CR_ADCAL);
    while (regs->CR & ADC_CR_ADCAL) {
    }
    SET_BIT(regs->CR, ADC_CR_ADEN);
    while (!(regs->ISR & ADC_ISR_ADRDY)) {
    }
    // Software-triggered conversion mode.
    regs->CFGR = ADC_CFGR_JQDIS | (res_val << ADC_CFGR_RES_Pos);
    // Measure Vref on first call.
    if (stm32_adc_get_vref() == -1) {
      stm32_adc_set_vref_from_vrefint();
    }
  }
  return true;
}

bool stm32_adc_enable_input(int unit_no, int input_no) {
  volatile ADC_TypeDef *regs = stm32_adc_get_regs(unit_no);
  if (regs == NULL) return false;
  const struct stm32_adc_input_def *ind = stm32_adc_get_input_def(input_no);
  if (ind == NULL) return false;
  // Sampling time of at least 5 uS is required for Vrefint and Vts.
  // We use it for external sources as well.
  // ADCCLK = 80 mhz / 10 * 47.5 = ~6 uS
  uint32_t smp_sel = 10;
  if (input_no < 10) {
    MODIFY_REG(regs->SMPR1, (7 << (input_no * 3)), (smp_sel << (input_no * 3)));
  } else {
    MODIFY_REG(regs->SMPR2, (7 << ((input_no - 10) * 3)),
               (smp_sel << ((input_no - 10) * 3)));
  }
  switch (ind->pin) {
    case STM32_ADC_PIN_VREFINT:
      SET_BIT(ADC123_COMMON->CCR, ADC_CCR_VREFEN);
      return true;
    case STM32_ADC_PIN_TEMP:
      SET_BIT(ADC123_COMMON->CCR, ADC_CCR_TSEN);
      return true;
    case STM32_ADC_PIN_VBAT:
      SET_BIT(ADC123_COMMON->CCR, ADC_CCR_VBATEN);
      return true;
  }
  return stm32_gpio_set_mode_analog(ind->pin, true /* adc */);
}

int stm32_adc_read(int unit_no, int input_no) {
  volatile ADC_TypeDef *regs = stm32_adc_get_regs(unit_no);
  if (regs == NULL) return -1;
  const struct stm32_adc_input_def *ind = stm32_adc_get_input_def(input_no);
  if (ind == NULL) return -1;
  // 1 conversion (L = 0), SQ1 = input_no.
  regs->SQR1 = (input_no << ADC_SQR1_SQ1_Pos);
  SET_BIT(regs->CR, ADC_CR_ADSTART);
  while (regs->CR & ADC_CR_ADSTART) {
  }
  return regs->DR;
}

int stm32_adc_set_vref_from_vrefint(void) {
  if (!mgos_adc_enable(STM32_ADC_PIN_VREFINT)) return -1;
  int mref = mgos_adc_read(STM32_ADC_PIN_VREFINT);
  stm32_adc_set_vref(
      __HAL_ADC_CALC_VREFANALOG_VOLTAGE(mref, ADC_RESOLUTION_12B));
  return stm32_adc_get_vref();
}

int stm32_adc_read_temp_c(void) {
  return __LL_ADC_CALC_TEMPERATURE(stm32_adc_get_vref(),
                                   mgos_adc_read(STM32_ADC_PIN_TEMP),
                                   ADC_RESOLUTION_12B);
}

bool mgos_adc_init(void) {
  // Enable SYSCLK/10 to all ADCs, no prescaler.
  MODIFY_REG(ADC123_COMMON->CCR, ADC_CCR_CKMODE, 0);
  MODIFY_REG(ADC123_COMMON->CCR, ADC_CCR_PRESC, 4 << ADC_CCR_PRESC_Pos);
  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_ADCSEL, RCC_ADCCLKSOURCE_SYSCLK);
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADCEN);
  return true;
}
#endif  // defined(STM32L4)
