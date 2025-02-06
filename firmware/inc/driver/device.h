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

#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdint.h>

typedef struct
{
    uint8_t dev_id;
    uint8_t dev_ss;
    uint8_t* dev_port;
} t_dev;

#endif
