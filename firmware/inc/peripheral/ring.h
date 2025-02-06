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

#ifndef RING_H_
#define RING_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t *buf;
    uint8_t *end;
    uint8_t *write;
    uint8_t *read;
} ring_t;

#define RING_INIT(buf_, len_) \
    ((ring_t){ \
    .buf = (buf_), \
    .end = (buf_)+(len_), \
    .write = (buf_), \
    .read = (buf_)} \
    )

ring_t ring_init(
    uint8_t *buf,
    size_t len
);

bool ring_is_empty(
    ring_t ring
);

bool ring_is_full(
    ring_t ring
);

size_t ring_push_available(
    ring_t ring
);

size_t ring_pop_available(
    ring_t ring
);

bool ring_push(
    ring_t *ring,
    uint8_t data
);

bool ring_push_over(
    ring_t *ring,
    uint8_t data
);

bool ring_pop(
    ring_t *ring,
    uint8_t *data
);

bool ring_peek(
    ring_t *ring,
    uint8_t *data
);

#endif
