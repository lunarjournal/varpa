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

#include "peripheral/timer.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

uint32_t millis_count = 0;

void timer_init(void)
{
    uint32_t ctc_overflow;

    ctc_overflow = ((F_CPU / 1000) / 8); 
    TCCR1B |= (1 << WGM12) | (1 << CS11);

    OCR1AH = (ctc_overflow >> 8);
    OCR1AL = ctc_overflow;

    TIMSK1 |= (1 << OCIE1A);
}

void timer_reset(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        millis_count = 0;
    }
}

uint32_t timer_millis(void)
{   
    uint32_t millis;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    millis = millis_count;
  }
  return millis;
}

ISR(TIMER1_COMPA_vect)
{
    millis_count++;
}
