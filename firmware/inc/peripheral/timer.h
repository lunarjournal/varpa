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

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

void timer_init(void);
void timer_reset(void);
uint32_t timer_millis(void);

#endif
