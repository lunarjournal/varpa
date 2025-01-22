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

#include "status.h"
#include <avr/pgmspace.h>

t_gate_status gate_status;
t_drain_status drain_status;
t_system_status system_status;

const char* const gate_lut[] PROGMEM =
{
    "INACTIVE",
    "PROBE_ERR",
    "PROBE_OK",
    "PROBE_TIME"
};

const char* const drain_lut[] PROGMEM =
{
    "INACTIVE",
    "POWER_SET",
    "OVERRIDE",
    "ADC_ERROR",
    "AMP_ALARM"
};

const char* const system_lut[] PROGMEM =
{
    "IDLE",
    "BUSY",
    "LOCKED",
    "SET_FAIL",
    "SET_REACHED",
    "SET_TIME",
    "VSWR_ALARM"
};
