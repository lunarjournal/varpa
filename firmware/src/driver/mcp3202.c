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

#include "peripheral/spi.h"
#include "driver/mcp3202.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void adc_init(void)
{
    MCP3202_DEV_0_DDR |= (1 << MCP3202_DEV_0_SS);
    MCP3202_DEV_1_DDR |= (1 << MCP3202_DEV_1_SS);

    MCP3202_DEV_0_PORT &= ~(1 << MCP3202_DEV_0_SS);
    MCP3202_DEV_1_PORT &= ~(1 << MCP3202_DEV_1_SS);

    _delay_ms(TRANSACT_DELAY_MS);

    MCP3202_DEV_0_PORT |= (1 << MCP3202_DEV_0_SS);
    MCP3202_DEV_1_PORT |= (1 << MCP3202_DEV_1_SS);

    _delay_ms(TRANSACT_DELAY_MS);
}

uint16_t adc_read(
    uint8_t id,
    uint8_t conv_bypass
)
{
    uint8_t dev_id = 0x0;
    uint8_t dev_ch = 0x0;
    uint8_t dev_ss = 0x0;
    uint8_t* dev_port = 0x0;
    uint8_t cmd_buf[3] = {0x0, 0x0, 0x0};
    uint16_t adc_daq = 0x0;

    switch(id)
    {
        case MCP3202_ID_FW_POWER:
            dev_id = MCP3202_DEV_FW_POWER;
            dev_ch = MCP3202_CHN_FW_POWER;
            break;
        
        case MCP3202_ID_REV_POWER:
            dev_id = MCP3202_DEV_REV_POWER;
            dev_ch = MCP3202_CHN_REV_POWER;
            break;

        case MCP3202_ID_DRAIN_VOLT:
            dev_id = MCP3202_DEV_DRAIN_VOLT;
            dev_ch = MCP3202_CHN_DRAIN_VOLT;
            break;

        case MCP3202_ID_DRAIN_AMP:
            dev_id = MCP3202_DEV_DRAIN_AMP;
            dev_ch = MCP3202_CHN_DRAIN_AMP;
            break;
    }

    switch(dev_id)
    {
        case MCP3202_DEV_0:
            dev_port = (uint8_t*)&MCP3202_DEV_0_PORT;
            dev_ss = MCP3202_DEV_0_SS;
            break;
        
        case MCP3202_DEV_1:
            dev_port = (uint8_t*)&MCP3202_DEV_1_PORT;
            dev_ss = MCP3202_DEV_1_SS;
            break;
    }

    cmd_buf[0] = MCP3202_REQ_START;
    cmd_buf[1] = (dev_ch == MCP3202_CHN_0) ? MCP3202_REQ_CH0 : MCP3202_REQ_CH1;
    cmd_buf[2] = MCP3202_REQ_PAD;

    spi_start(cmd_buf, cmd_buf, sizeof(cmd_buf), dev_port, dev_ss);
    spi_flush();

    adc_daq = (uint16_t)(cmd_buf[MCP3202_DAQ_MSB]);
    adc_daq |= ((uint16_t)(cmd_buf[MCP3202_DAQ_LSB] & 0xF) << 8);
    adc_daq = adc_daq & 0xFFF;

    if (conv_bypass == 0)
    {
        switch(id)
        {
            case MCP3202_ID_DRAIN_VOLT:
                adc_daq = (uint32_t)(((uint32_t)adc_daq * MCP3202_DRAIN_VOLT_GAIN)/10);
                break;
            case MCP3202_ID_DRAIN_AMP:
                adc_daq = (uint32_t)(((uint32_t)adc_daq * 10)/MCP3202_DRAIN_AMP_GAIN);
                break;
        }
    }
        
    return (uint16_t)adc_daq;
}

uint16_t adc_read_n(
    uint8_t id,
    uint8_t n
)
{
    uint32_t x = 0;
    uint8_t i = 0;

    for (i = 0; i < n; i++)
    {
        x += adc_read(id, 0);
    }

    x /= n;

    return (uint16_t)x;
}
