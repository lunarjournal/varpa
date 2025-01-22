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

#include "util.h"

#include <stdio.h>
#include <avr/pgmspace.h>

extern int __heap_start, *__brkval;

const uint16_t fw_pwr_gradient[] = { 249, 356, 374, 388, 400, 402, 435, 460, 500, 553 };
const int16_t fw_pwr_intercept[] = { -5418, -3810, -3602, -3431, -3289, -3261, -2877, -2601, -2189, -1725 };
const uint16_t fw_pwr_threshold[] = { 1288, 1548, 1704, 1812, 1897, 1967, 2164, 2301, 2411, 2505 };

uint8_t util_volt_to_count(
    uint16_t volt,
    uint16_t* count
)
{
    uint8_t bits = 0x0;
    uint32_t x = 0x0;
    uint32_t y = 0x0;
    uint16_t i = 0x0;

    bits = UTIL_CONV_BITS;

    if (volt > UTIL_SYSTEM_VOLT)
    {
        return 1;
    }

    for (i = 0; i < bits; i++)
    {
        x |= UTIL_CONV_BIT(i);
    }

    y = ((uint32_t)volt) * x;
    y /= UTIL_SYSTEM_VOLT;

    *count = (uint16_t)y;
    return 0;
}

uint8_t util_count_to_volt(
    uint16_t count,
    uint16_t* volt
)
{
    uint8_t bits = 0x0;
    uint32_t x = 0x0;
    uint32_t y = 0x0;
    uint16_t i = 0x0;

    bits = UTIL_CONV_BITS;

    for (i = 0; i < bits; i++)
    {
        x |= UTIL_CONV_BIT(i);
    }

    if (count > x)
    {
        return 1;
    }

    y = ((uint32_t)count) * UTIL_SYSTEM_VOLT;
    y /= x;

    *volt = (uint16_t)y;
    return 0;
}

int8_t util_count_to_pwr(
    uint16_t count,
    int16_t* cpwr
)
{
    uint32_t x = 0x0;
    int16_t y = 0x0;
    uint16_t slope = 0x0;
    int16_t intercept = 0x0;
    uint16_t i = 0;

    uint8_t pwr_cal_size = sizeof(fw_pwr_gradient) / sizeof(uint16_t);
    uint8_t pwr_last_element = pwr_cal_size - 1;

    if(count >= fw_pwr_threshold[pwr_last_element])
    {
        slope = fw_pwr_gradient[pwr_last_element];
        intercept = fw_pwr_intercept[pwr_last_element];
    }
    else if (count <= fw_pwr_threshold[0])
    {
        slope = fw_pwr_gradient[0];
        intercept = fw_pwr_intercept[0];
    }
    else
    {
        for(i = 0; i < pwr_cal_size; i++)
        {
            if(count < fw_pwr_threshold[i])
            {
                slope = fw_pwr_gradient[i];
                intercept = fw_pwr_intercept[i];
                break;
            }
        }
    }

    x = (((uint32_t)count * 1000) / slope);
    y = (int16_t)x;

    y = y + intercept;
    *cpwr = y;

    return 0;
}

void util_print_pwr(
    int16_t cpwr
)
{
    int16_t integer_part = cpwr / 100;
    
    int16_t fractional_part = cpwr % 100;

    if (fractional_part < 0) {
        fractional_part = -fractional_part;
    }

    printf("%d.%02d dBm\n", integer_part, fractional_part);
}

uint16_t util_get_free_mem(void)
{
    uint16_t free_mem;
    if ((int)__brkval == 0) {
        free_mem = (int)&free_mem - (int)&__heap_start;
    } else {
        free_mem = (int)&free_mem - (int)__brkval;
    }
    return free_mem;
}
