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

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#if SPI_MODE == 0
    #define CPOL_VALUE 0
    #define CPHA_VALUE 0
#elif SPI_MODE == 1
    #define CPOL_VALUE 0
    #define CPHA_VALUE 1
#elif SPI_MODE == 2
    #define CPOL_VALUE 1
    #define CPHA_VALUE 0
#elif SPI_MODE == 3
    #define CPOL_VALUE 1
    #define CPHA_VALUE 1
#else
    #error "No valid SPI_MODE defined!"
#endif

#if SPI_DORD == 0
    #define DORD_VALUE 0
#elif SPI_DORD == 1
    #define DORD_VALUE 1
#else
    #error "No valid SPI_DORD defined!"
#endif

static volatile uint8_t *spi_out;
static volatile uint8_t *spi_in;
static volatile size_t spi_len;
static volatile uint8_t *spi_port;
static volatile uint8_t spi_pin;

void spi_init(void)
{
    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
    
    SPSR |= (SPI2X_VALUE << SPI2X);
    SPCR |= (1 << SPIE) | (1 << SPE) | (DORD_VALUE << DORD) | (1 << MSTR)
        | (CPOL_VALUE << CPOL) | (CPHA_VALUE << CPHA)
        | (SPR1_VALUE << SPR1) | (SPR0_VALUE << SPR0);
}

bool spi_busy(void)
{
    return spi_len;
}

void spi_flush(void)
{
    while(spi_len);
}

void spi_start(
    uint8_t *out,
    uint8_t *in,
    size_t len,
    uint8_t *port,
    uint8_t pin
)
{
    spi_flush();
    
    spi_out = out;
    spi_in = in;
    spi_len = len;
    spi_port = port;
    spi_pin = pin;
    
    if(spi_port)
        *spi_port &= ~(1 << spi_pin);
    
    SPDR = *spi_out++;
}

ISR(SPI_STC_vect)
{
    if(spi_in)
    {
        *spi_in++ = SPDR;
    }
    
    if(--spi_len)
        SPDR = *spi_out++;
    else
        if(spi_port)
            *spi_port |= (1 << spi_pin);
}
