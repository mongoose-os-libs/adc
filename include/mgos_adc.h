/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/*
 * ADC API.
 *
 * See https://en.wikipedia.org/wiki/Analog-to-digital_converter for the
 * backround information.
 */

#ifndef CS_MOS_LIBS_ADC_SRC_MGOS_ADC_H_
#define CS_MOS_LIBS_ADC_SRC_MGOS_ADC_H_

#include <stdbool.h>

#if CS_PLATFORM == CS_P_ESP32
#include "esp32/esp32_adc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Configure and enable ADC */
bool mgos_adc_enable(int pin);

/* Read from the analog pin. Returns raw value. */
int mgos_adc_read(int pin);

/*
 * Read from the specified analog pin.
 * Returns voltage on the pin, in mV.
 */
int mgos_adc_read_voltage(int pin);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MOS_LIBS_ADC_SRC_MGOS_ADC_H_ */
