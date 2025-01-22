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

#include "peripheral/spi.h"

#include "driver/si4468.h"
#include "driver/radio_config.h"
#include "driver/device.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>


static const uint8_t config[] PROGMEM = RADIO_CONFIGURATION_DATA_ARRAY;

void si4468_init(void)
{
    SI4468_DEV_0_DDR |= (1 << SI4468_DEV_0_SS);
    SI4468_DEV_0_PORT |= (1 << SI4468_DEV_0_SS);
}

void si44468_get_device(
    int id,
    t_dev* device
)
{
    uint8_t dev_id = 0x0;
    uint8_t dev_ss = 0x0;
    uint8_t* dev_port = 0x0;

    device->dev_id = 0;
    device->dev_ss = 0;
    device->dev_port = 0;

     switch (id)
        {
            case SI4468_ID_RADIO:
                dev_id = SI4468_DEV_RADIO;
                break;
        }

        switch (dev_id)
        {
            case SI4468_DEV_RADIO:
                dev_ss = SI4468_DEV_0_SS;
                dev_port = (uint8_t*)&SI4468_DEV_0_PORT;
            break;
        }

    device->dev_id = dev_id;
    device->dev_ss = dev_ss;
    device->dev_port = dev_port;
}

static uint8_t si4468_get_response(
    void* buf,
    uint8_t len
)
{
    t_dev device;
    uint8_t cts = 0x0;
    uint8_t* cmd_buf = 0x0;
    uint16_t cmd_size = 0x0;
    uint8_t i = 0;

    si44468_get_device(SI4468_ID_RADIO, &device);

    cmd_size = len + SI4468_GET_RESP_LEN;
    cmd_buf = (uint8_t*)malloc((cmd_size));

    cmd_buf[0] = SI4468_CMD_READ_CMD_BUFF;
    cmd_buf[1] = SI4468_CTS;

    spi_start(cmd_buf, cmd_buf, SI4468_GET_RESP_LEN,  device.dev_port, device.dev_ss);
    spi_flush();

    cts = (cmd_buf[0] == SI4468_CTS);

    if(cts)
    {
        if (len > 0)
        {
            for(i = 0x0; i < cmd_size; i++)
            {
                cmd_buf[i] = SI4468_CTS;
            }

            cmd_buf[0] = SI4468_CMD_READ_CMD_BUFF;

            spi_start(cmd_buf, cmd_buf, cmd_size, (uint8_t*)&SI4468_DEV_0_PORT,  SI4468_DEV_0_SS);
            spi_flush();

            for(i = SI4468_GET_RESP_LEN; i < cmd_size; i++)
            {
                ((uint8_t*)buf)[i - SI4468_GET_RESP_LEN] = cmd_buf[i];
            }

        }
    }

    free(cmd_buf);

    return cts;
}

static uint8_t si4468_wait_for_response(
    void* buf,
    uint8_t buf_len
)
{
    while(!si4468_get_response(buf, buf_len))
    {
        _delay_ms(10);
    }

    return 1;
}

static void si4468_exec_api(
    void* in, 
    size_t in_len,
    void* out,
    uint8_t out_len
)
{
    t_dev device;
    si44468_get_device(SI4468_ID_RADIO, &device);

    if (si4468_wait_for_response(0, 0))
    {
        spi_start(in, 0, in_len, device.dev_port,  device.dev_ss);
        spi_flush();

        if (out != 0)
        {
            si4468_wait_for_response(out, out_len);
        }
    }
}

void si4468_apply_startup_config(void)
{
    uint16_t i = 0;
    uint8_t buf[100];

    for(i = 0; i < sizeof(config); i++)
    {
        memcpy_P(buf, &config[i], sizeof(buf));
        uint8_t tmp = buf[0];
        si4468_exec_api(&buf[1], tmp, 0, 0);
        i += tmp;
    }
}

void si4468_get_info(
    si4468_info_t* info
)
{
    uint8_t data[8] = {
		SI4468_CMD_PART_INFO
	};

    si4468_exec_api(data, 1, data, 8);

	info->chipRev	= data[0];
	info->part		= (data[1]<<8) | data[2];
	info->partBuild	= data[3];
	info->id		= (data[4]<<8) | data[5];
	info->customer	= data[6];
	info->romId		= data[7];
}

void si4468_tx_mode(void)
{
    uint8_t packet[] = {SI4468_CMD_START_TX, 0,0,0, 0,0,0 };
    si4468_exec_api(packet, sizeof(packet), 0, 0);
}

void si4468_rx_mode(void)
{
    uint8_t packet[] = {SI4468_CMD_CHANGE_STATE, 0x3};
    si4468_exec_api(packet, sizeof(packet), 0, 0);
}
