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

#ifndef SI4468_H_
#define SI4468_H_

#include "setup.h"

#include <stdint.h>
#include <avr/io.h>

#define SI4468_ID_RADIO 0xD1

#define SI4468_CMD_READ_CMD_BUFF 0x44
#define SI4468_CMD_PART_INFO 0x01
#define SI4468_CMD_FUNC_INFO 0x10
#define SI4468_CMD_SET_PROPERTY	0x11
#define SI4468_CMD_IRCAL 0x17
#define SI4468_CMD_START_TX	0x31
#define SI4468_CMD_CHANGE_STATE	0x34

#define SI4468_CTS 0xFF
#define SI4468_GET_RESP_LEN 0x2

#define SI4468_DEV_0 0
#define SI4468_DEV_RADIO SI4468_DEV_0
#define SI4468_DEV_0_PORT PORTB
#define SI4468_DEV_0_DDR DDRB
#define SI4468_DEV_0_SS PORTB2

typedef struct {
	uint8_t chipRev;
	uint16_t part;
	uint8_t partBuild;
	uint16_t id;
	uint8_t customer;
	uint8_t romId; ///< ROM ID (3 = revB1B, 6 = revC2A)
} si4468_info_t;

void si4468_init(void);
void si4468_get_info(si4468_info_t* info);
void si4468_apply_startup_config(void);
void si4468_tx_mode(void);
void si4468_rx_mode(void);
#endif
