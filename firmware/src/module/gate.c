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

#include "setup.h"

#include "driver/mcp3202.h"
#include "driver/mcp4725.h"

#include "module/gate.h"

#include "util.h"
#include "status.h"

#include <avr/interrupt.h>
#include <util/delay.h>

extern t_gate_status gate_status;
extern t_system_status system_status;

uint16_t gate_probe(
    uint16_t target_amp,
    uint16_t* probe_result
)
{
    uint16_t probe_volt = 0x0;
    uint16_t drain_amp = 0x0;
    uint16_t gate_count = 0x0;
    uint16_t drain_count = 0x0;

    system_status.status = SYSTEM_STATUS_BUSY;

    util_volt_to_count(GATE_PROBE_DRAIN, &drain_count);
    util_volt_to_count(GATE_PROBE_START, &gate_count );
    
    dac_write(MCP4725_ID_GATE_VOLT, gate_count, 0);
    _delay_ms(TRANSACT_DELAY_MS);
    dac_write(MCP4725_ID_DRAIN_VOLT, drain_count, 0);
    _delay_ms(TRANSACT_DELAY_MS);

    while((drain_amp < target_amp) && probe_volt < GATE_PROBE_LIMIT)
    {
        gate_count += GATE_PROBE_INC;
        dac_write(MCP4725_ID_GATE_VOLT, gate_count, 0);
        _delay_us(GATE_LOOP_DELAY_US);
        
        drain_count = adc_read(MCP3202_ID_DRAIN_AMP, 0);
        util_count_to_volt(drain_count, &drain_amp);
        util_count_to_volt(gate_count, &probe_volt);
    }

    _delay_ms(TRANSACT_DELAY_MS);
    drain_count = adc_read(MCP3202_ID_DRAIN_AMP, 0);
    util_count_to_volt(drain_count, &drain_amp);

    if(GATE_PA_POWER_DOWN)
    {
        _delay_ms(TRANSACT_DELAY_MS);
        dac_write(MCP4725_ID_DRAIN_VOLT, 0, 0);
    }

    gate_status.q_amp = drain_amp;
    gate_status.q_volt = probe_volt;

    *probe_result = probe_volt;

    system_status.status = SYSTEM_STATUS_IDLE;

    return drain_amp;
}
