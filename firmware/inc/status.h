/**
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

#ifndef STATUS_H_
#define STATUS_H_

#include <stdint.h>
#include <avr/pgmspace.h>

extern const char* const gate_lut[];
extern const char* const drain_lut[];
extern const char* const system_lut[];

enum
{
    GATE_STATUS_INACTIVE = 0,
    GATE_STATUS_PROBE_ERROR,
    GATE_STATUS_PROBE_SUCCESS,
    GATE_STATUS_PROBE_TIMEOUT
};

typedef struct {
    uint8_t status;
    uint16_t q_volt;
    uint16_t q_amp;
    uint16_t q_time;
    uint16_t v_set;
    uint16_t q_drain;
    uint8_t func;
} t_gate_status;

enum
{
    DRAIN_STATUS_INACTIVE = 0,
    DRAIN_STATUS_POWER_CONTROL,
    DRAIN_STATUS_OVERRIDE,
    DRAIN_STATUS_ADC_ERROR,
    DRAIN_STATUS_CURRENT_ALARM
};

typedef struct {
    uint8_t status;
    uint16_t v_set;
    uint16_t v_adc;
    uint16_t amp;
    uint8_t func;
} t_drain_status;

enum
{
    SYSTEM_STATUS_IDLE = 0,
    SYSTEM_STATUS_BUSY,
    SYSTEM_STATUS_LOCKED,
    SYSTEM_STATUS_SETPOINT_FAIL,
    SYSTEM_STATUS_SETPOINT_REACHED,
    SYSTEM_STATUS_SETPOINT_TIMEOUT,
    SYSTEM_STATUS_VSWR_ALARM
};

typedef struct {
    uint8_t status;
    uint16_t v_fw;
    int16_t p_fw;
    uint16_t v_rev;
    int16_t p_rev;
    uint16_t swr;
    uint16_t swr_lim;
    uint16_t atten;
    uint8_t p_strat;
    int16_t p_set;
    uint16_t d_time;
} t_system_status;

#endif
