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

#include "peripheral/twi.h"
#include "driver/mcp4725.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void dac_init(uint16_t init_value)
{
        dac_write(MCP4725_ID_GATE_VOLT, init_value, 0);
        dac_write(MCP4725_ID_DRAIN_VOLT, init_value, 0);

        _delay_ms(TRANSACT_DELAY_MS);
}

void dac_write(
    uint8_t id,
    uint16_t value,
    uint8_t conv_bypass
)
{
    uint8_t dev_addr = 0x0;
    uint8_t cmd_buf[2] = {0x0, 0x0};
    uint8_t drain_mode = 0x0;

    switch(id)
    {
        case MCP4725_ID_GATE_VOLT:
            dev_addr = TWI_ADDRESS_W(MCP4725_DEV_GATE_VOLT_ADDR);
            break;
        case MCP4725_ID_DRAIN_VOLT:
            dev_addr = TWI_ADDRESS_W(MCP4725_DEV_DRAIN_VOLT_ADDR);
            drain_mode = 1;
            break;
    }

    if (conv_bypass == 0)
    {
        if (drain_mode == 1)
        {
            value = (uint16_t)(((uint32_t)value * 10)/MCP4725_DRAIN_GAIN);
        }
    }

    cmd_buf[0] = ((value >> 8) & 0xF) | MCP4725_BIT_PD0(0) | MCP4725_BIT_PD1(0);
    cmd_buf[1] = value & 0xFF;

    twi_start(dev_addr, cmd_buf, sizeof(cmd_buf));
    twi_flush();
}

uint16_t dac_read(
    uint8_t id,
    uint8_t conv_bypass
)
{
    uint8_t dev_addr = 0x0;
    uint8_t cmd_buf[3] = {0x0, 0x0, 0x0};
    uint16_t value = 0x0;
    uint8_t drain_mode = 0x0;

    switch(id)
    {
        case MCP4725_ID_GATE_VOLT:
            dev_addr = TWI_ADDRESS_R(MCP4725_DEV_GATE_VOLT_ADDR);
            break;
        case MCP4725_ID_DRAIN_VOLT:
            dev_addr = TWI_ADDRESS_R(MCP4725_DEV_DRAIN_VOLT_ADDR);
            drain_mode = 1;
            break;
    }

    twi_start(dev_addr, cmd_buf, sizeof(cmd_buf));
    twi_flush();

    value = (((uint16_t)cmd_buf[1]) << 4) | (cmd_buf[2] >> 4);

    if (conv_bypass == 0)
    {
        if (drain_mode)
        {
            value = (uint16_t)(((uint32_t)value * MCP4725_DRAIN_GAIN)/10);
        }
    }

    return (value & 0xFFF); 
}
