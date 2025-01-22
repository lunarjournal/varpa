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

#ifndef F1956_H_
#define F1956_H_

#include "setup.h"

#include <stdint.h>
#include <avr/io.h>

#define F1956_ID_INPUT_ATT 0xC1

#define F1956_DEV_0 0
#define F1956_DEV_INPUT_ATT F1956_DEV_0

#define F1956_DEV_INPUT_ATT_ADDR 0x0

#define F1956_DEV_0_PORT PORTC
#define F1956_DEV_0_DDR DDRC
#define F1956_DEV_0_SS PORTC2

void att_init(
    uint8_t init_value
);

void att_set(
    uint8_t id,
    uint8_t value
);

#endif
