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

#ifndef SPI_H_
#define SPI_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void spi_init(void);

bool spi_busy(void);

void spi_flush(void);

void spi_start(
    uint8_t *out,
    uint8_t *in,
    size_t len,
    uint8_t *port,
    uint8_t pin
);

#endif
