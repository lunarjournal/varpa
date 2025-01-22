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

#ifndef DRAIN_H_
#define DRAIN_H_

#include <stdint.h>

#define DRAIN_PROBE_START 0 // mV
#define DRAIN_PROBE_LIMIT 3000
#define DRAIN_PROBE_INC_FINE 1
#define DRAIN_PROBE_INC_COARSE 5
#define DRAIN_PROBE_INC_THRESHOLD 100
#define DRAIN_Q_AMP 100 // mA
#define DRAIN_TEST_COUNT 10
#define DRAIN_TEST_BOUNDARY 10
#define DRAIN_POWER_SAMPLES 10
#define DRAIN_LOOP_DELAY_US 1000
#define DRAIN_SETPOINT_TIMEOUT_MS 1500
#define DRAIN_POWER_LOW_LIM 0
#define DRAIN_POWER_HIGH_LIM 3000
#define DRAIN_POWER_MODULUS 50

uint16_t drain_sweep_pwr(
    int16_t target_fw_pwr
);

#endif
