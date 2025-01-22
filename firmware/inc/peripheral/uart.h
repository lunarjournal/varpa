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

#ifndef UARTINT_H_
#define UARTINT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

extern FILE uart_out;
extern FILE uart_in;

void uart_init(void);

bool uart_tx(
    uint8_t data
);

bool uart_rx(
    uint8_t* data
);

size_t uart_tx_burst(
    uint8_t* data,
    size_t size
);

size_t uart_rx_burst(
    uint8_t* data,
    size_t size
);

size_t uart_tx_available(void);

size_t uart_rx_available(void);

bool uart_rx_peek(
    uint8_t* data
);

void uart_tx_flush(void);

char *uart_ngets(char *s, size_t n);

#endif
