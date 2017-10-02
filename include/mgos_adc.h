/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/*
 * See on GitHub:
 * [mgos_adc.h](https://github.com/cesanta/mongoose-os/blob/master/mgos_adc.h),
 * [mgos_adc.c](https://github.com/cesanta/mongoose-os/blob/master/mgos_adc.c)
 */

#ifndef CS_MOS_LIBS_ADC_SRC_MGOS_ADC_H_
#define CS_MOS_LIBS_ADC_SRC_MGOS_ADC_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Configure and enable ADC */
bool mgos_adc_enable(int pin);

/* Read from the analog pin */
int mgos_adc_read(int pin);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MOS_LIBS_ADC_SRC_MGOS_ADC_H_ */
