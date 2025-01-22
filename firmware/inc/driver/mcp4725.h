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

#ifndef MCP4725_H_
#define MCP4725_H_

#define MCP4725_BIT_PD0(x) (x << 4)
#define MCP4725_BIT_PD1(x) (x << 5)

#define MCP4725_ID_GATE_VOLT 0xB1
#define MCP4725_ID_DRAIN_VOLT 0xB2

#define MCP4725_DEV_0 0
#define MCP4725_DEV_1 1

#define MCP4725_DEV_GATE_VOLT MCP4725_DEV_0
#define MCP4725_DEV_DRAIN_VOLT MCP4725_DEV_1

#define MCP4725_DEV_GATE_VOLT_ADDR 0x60
#define MCP4725_DEV_DRAIN_VOLT_ADDR 0x61

#define MCP4725_DRAIN_GAIN 23

void dac_init(
    uint16_t init_value
);

void dac_write(
    uint8_t id,
    uint16_t value,
    uint8_t conv_bypass
);

uint16_t dac_read(
    uint8_t id,
    uint8_t conv_bypass
);

#endif
