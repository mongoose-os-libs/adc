/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/*
 * ADC API specific to the ESP32 platform.
 */

#ifndef CS_MOS_LIBS_ADC_INCLUDE_ESP32_ESP32_ADC_H_
#define CS_MOS_LIBS_ADC_INCLUDE_ESP32_ESP32_ADC_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Set ADC voltage reference, used to convert raw values to voltage.
 * See
 * https://esp-idf.readthedocs.io/en/latest/api-reference/peripherals/adc.html#adc-calibration
 * for more details.
 * By default, 1.1V Vref is assumed, i.e. perfectly accurate.
 * Must be called before any ADC channels are configured.
 */
void esp32_adc_set_vref(int vref_mv);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MOS_LIBS_ADC_INCLUDE_ESP32_ESP32_ADC_H_ */
