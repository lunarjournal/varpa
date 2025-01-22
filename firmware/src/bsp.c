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
#include "bsp.h"

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

void bsp_soft_reset(void)
{
    cli();

    MCUSR &= ~(1 << WDRF);

    WDTCSR = (1 << WDCE) | (1 << WDE);
    WDTCSR = (1 << WDP1);
    WDTCSR = WDTCSR | 0x40;

    MCUSR &= ~(1 << WDRF);

    sei();

    while(1){};
}
