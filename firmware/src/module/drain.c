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

#include "setup.h"

#include "peripheral/timer.h"

#include "driver/mcp3202.h"
#include "driver/mcp4725.h"
#include "driver/si4468.h"

#include "module/gate.h"
#include "module/drain.h"

#include "util.h"
#include "status.h"

#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

extern t_drain_status drain_status;
extern t_system_status system_status;

uint16_t drain_sweep_pwr(
    int16_t target_fw_pwr
)
{
    uint16_t drain_count = 0x0;
    uint16_t drain_inc = 0x0;
    uint16_t fw_pwr_adc = 0x0;
    int16_t fw_pwr = 0x0;
    uint16_t test_count = 0x0;
    int16_t test_boundary = DRAIN_TEST_BOUNDARY;

    system_status.status = SYSTEM_STATUS_BUSY;
    drain_status.status = DRAIN_STATUS_POWER_CONTROL;

    drain_count = dac_read(MCP4725_ID_DRAIN_VOLT, 1);
    _delay_ms(TRANSACT_DELAY_MS);

    si4468_tx_mode();
    _delay_ms(TRANSACT_DELAY_MS);

    dac_write(MCP4725_ID_DRAIN_VOLT, drain_count, 1);
    _delay_ms(TRANSACT_DELAY_MS);

    fw_pwr_adc = adc_read_n(MCP3202_ID_FW_POWER, 10);
    util_count_to_pwr(fw_pwr_adc, &fw_pwr);

    _delay_ms(TRANSACT_DELAY_MS);

    timer_reset();
    uint32_t time_start = timer_millis();
    bool timeout = false;

    while ((test_count < DRAIN_TEST_COUNT) && 
           (drain_count < DRAIN_PROBE_LIMIT))
    {
        if ((timer_millis() - time_start) > DRAIN_SETPOINT_TIMEOUT_MS)
        {
            timeout = true;
            break;
        }

        if (abs(fw_pwr - target_fw_pwr) > DRAIN_PROBE_INC_THRESHOLD)
        {
            drain_inc = DRAIN_PROBE_INC_COARSE;
        }
        else
        {
            drain_inc = DRAIN_PROBE_INC_FINE;
        }
            
        if (fw_pwr < (target_fw_pwr))
        {
            drain_count += drain_inc;
        }
        else if (fw_pwr > (target_fw_pwr))
        {
            drain_count -= drain_inc;
        }

        dac_write(MCP4725_ID_DRAIN_VOLT, drain_count, 1);
        _delay_us(DRAIN_LOOP_DELAY_US);

        fw_pwr_adc = adc_read_n(MCP3202_ID_FW_POWER, DRAIN_POWER_SAMPLES);
        util_count_to_pwr(fw_pwr_adc, &fw_pwr);

        if (fw_pwr <= (target_fw_pwr + test_boundary) &&
            fw_pwr >= (target_fw_pwr - test_boundary))
            {
                test_count += 1;
            }
    }

    system_status.d_time = (uint16_t)(timer_millis() - time_start);

    if (timeout)
    {
        system_status.status = SYSTEM_STATUS_SETPOINT_TIMEOUT;
    }
    else if(test_count >= DRAIN_TEST_COUNT)
    {
        system_status.status = SYSTEM_STATUS_SETPOINT_REACHED;
    }
    else
    {
        system_status.status = SYSTEM_STATUS_SETPOINT_FAIL;
    }

    return test_count;
}
