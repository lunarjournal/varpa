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
#include "driver/f1956.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void att_init(uint8_t init_value)
{
    F1956_DEV_0_DDR |= (1 << F1956_DEV_0_SS);
    F1956_DEV_0_PORT |= (1 << F1956_DEV_0_SS);

    att_set(F1956_ID_INPUT_ATT, init_value);
    _delay_ms(10);
}

void att_set(
    uint8_t id,
    uint8_t value
)
{
    uint8_t dev_id = 0x0;
    uint8_t dev_ss = 0x0;
    uint8_t* dev_port = 0x0;
    uint8_t cmd_buf[2] = {0x0, 0x0};

    switch (id)
    {
        case F1956_ID_INPUT_ATT:
            dev_id = F1956_DEV_INPUT_ATT;
            break;
    }

    switch (dev_id)
    {
        case F1956_DEV_INPUT_ATT:
            dev_ss = F1956_DEV_0_SS;
            dev_port = (uint8_t*)&F1956_DEV_0_PORT;
        break;
    }

    cmd_buf[0] = value & 0x7F;
    cmd_buf[1] = F1956_DEV_INPUT_ATT_ADDR;

    spi_start(cmd_buf, cmd_buf, sizeof(cmd_buf), dev_port, dev_ss);
    spi_flush();
}
