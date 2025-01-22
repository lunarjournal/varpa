/**
 * VARPA MSc Project
 * 
 * Author: Dylan Muller
 * Copyright (c) 2025 
 * All rights reserved.
 * 
 * - Commercial/IP use prohibited.
 * - Attribution required.
 * See License.txt
 *
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#define UTIL_SYSTEM_VOLT ((uint32_t)3300) // (mV)
#define UTIL_CONV_BITS 12
#define UTIL_CONV_BIT(x) (1 << x)

#define UTIL_COUPLER_ATTEN ((int16_t)4250) // BI-DIR COUPL CAL
#define UTIL_POWER_THRESHOLD 2000

uint8_t util_volt_to_count(
    uint16_t volt,
    uint16_t* count
);

uint8_t util_count_to_volt(
    uint16_t count,
    uint16_t* volt
);

int8_t util_count_to_pwr(
    uint16_t count,
    int16_t* cpwr
);

void util_print_pwr(
    int16_t cpwr
);

uint16_t util_get_free_mem(void);

#endif
