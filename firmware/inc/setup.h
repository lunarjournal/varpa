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

/* Global Peripheral Configuration*/

#define __AVR_ATmega328__

#define F_CPU 8000000UL

#define BAUD 19200
#define UART_BUF_SIZE 25
#define UART_OVERWRITE

#define TWI_FREQUENCY 350000
#define TWI_PRESCALER 1
#define TWPS0_VALUE 0
#define TWPS1_VALUE 0

#define SPI_FREQUENCY 500000
#define SPI_PRESCALER 16
#define SPR0_VALUE 1
#define SPR1_VALUE 0
#define SPI2X_VALUE 0
#define SPI_MODE 0
#define SPI_DORD 0

#define TRANSACT_DELAY_MS 5
#define INIT_DELAY_MS 10

#define SYS_VER_STR "1.01.5"
