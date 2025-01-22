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

#include "cmd.h"

#include <avr/pgmspace.h>

const char* const cmd_lut[CMD_SIZE][CMD_ENTRY_SIZE] PROGMEM = {
    {CMD_POWER_GET_STR, CMD_POWER_GET_DESC},
    {CMD_POWER_SET_STR, CMD_POWER_SET_DESC},
    {CMD_TX_STR, CMD_TX_DESC},
    {CMD_RX_STR, CMD_RX_DESC},
    {CMD_DRAIN_SET_STR, CMD_DRAIN_SET_DESC},
    {CMD_PROBE_STR, CMD_PROBE_DESC},
    {CMD_RESET_STR, CMD_RESET_DESC},
    {CMD_REPORT_STR, CMD_REPORT_DESC},
    {CMD_HELP_STR, CMD_HELP_DESC}
    };
