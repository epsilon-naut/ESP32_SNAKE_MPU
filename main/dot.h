#ifndef DOT_H
#define DOT_H

// A simple struct for the dots (snake head, snake tail, apple) used.

#include <stdlib.h>
#include <stdint.h>

typedef struct dot {
    uint8_t x;
    uint8_t y;
} dot;

#endif