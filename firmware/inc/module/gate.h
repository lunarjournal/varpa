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

#ifndef GATE_H_
#define GATE_H_

#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#define GATE_PROBE_START 2800 // mV
#define GATE_PROBE_LIMIT 3300 // mV
#define GATE_PROBE_DRAIN 1000 // mV
#define GATE_PROBE_INC 1 // COUNTS
#define GATE_PA_POWER_DOWN 1
#define GATE_LOOP_DELAY_US 1000

uint16_t gate_probe(
    uint16_t target_amp, // mA
    uint16_t* probe_result
);

#endif
