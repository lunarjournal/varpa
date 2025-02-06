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

#ifndef TWI_H_
#define TWI_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TWI_ADDRESS_W(x)    (((x) << 1) & ~0x01)
#define TWI_ADDRESS_R(x)    (((x) << 1) | 0x01)

void twi_init(void);

bool twi_busy(void);

void twi_flush(void);

void twi_start(
    uint8_t address,
    uint8_t *data,
    size_t len
);

#endif
