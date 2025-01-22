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

#ifndef MCP3202_H_
#define MCP3202_H_

#include <stdint.h>
#include <avr/io.h>

#define MCP3202_BIT_MSBBF(x) (x << 5)
#define MCP3202_BIT_ODD(x) (x << 6)
#define MCP3202_BIT_SGL(x) (x << 7)
#define MCP3202_BIT_START (1 << 0)

#define MCP3202_DAQ_MSB 0x2
#define MCP3202_DAQ_LSB 0x1

#define MCP3202_BUILD_REQ(MSBF, ODD, SGL) \
    (uint8_t)(MCP3202_BIT_MSBBF(MSBF) | \
              MCP3202_BIT_ODD(ODD) | \
              MCP3202_BIT_SGL(SGL))

#define MCP3202_REQ_START (uint8_t)(MCP3202_BIT_START)
#define MCP3202_REQ_CH0 MCP3202_BUILD_REQ(1, 0, 1)
#define MCP3202_REQ_CH1 MCP3202_BUILD_REQ(1, 1, 1)
#define MCP3202_REQ_PAD (uint8_t)(0x0)

#define MCP3202_ID_FW_POWER 0xA1
#define MCP3202_ID_REV_POWER 0xA2
#define MCP3202_ID_DRAIN_VOLT 0xA3
#define MCP3202_ID_DRAIN_AMP 0xA4

#define MCP3202_CHN_0 0
#define MCP3202_CHN_1 1

#define MCP3202_CHN_DRAIN_VOLT MCP3202_CHN_0
#define MCP3202_CHN_DRAIN_AMP MCP3202_CHN_1
#define MCP3202_CHN_FW_POWER MCP3202_CHN_1
#define MCP3202_CHN_REV_POWER MCP3202_CHN_0

#define MCP3202_DEV_0 0
#define MCP3202_DEV_1 1

#define MCP3202_DEV_DRAIN_VOLT MCP3202_DEV_0
#define MCP3202_DEV_DRAIN_AMP MCP3202_DEV_0
#define MCP3202_DEV_FW_POWER MCP3202_DEV_1
#define MCP3202_DEV_REV_POWER MCP3202_DEV_1

#define MCP3202_DEV_0_PORT PORTC
#define MCP3202_DEV_0_DDR DDRC
#define MCP3202_DEV_0_SS PORTC0
#define MCP3202_DEV_1_PORT PORTC
#define MCP3202_DEV_1_DDR DDRC
#define MCP3202_DEV_1_SS PORTC1

#define MCP3202_DRAIN_VOLT_GAIN 26
#define MCP3202_DRAIN_AMP_GAIN 25


void adc_init(void);

uint16_t adc_read(
    uint8_t id,
    uint8_t conv_bypass
);

uint16_t adc_read_n(
    uint8_t id,
    uint8_t n
);


#endif
