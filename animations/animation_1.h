#ifndef ANIMATION_1_H
#define ANIMATION_1_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

typedef struct {
    double frames[40][5][5];
} AnimationFrames;

uint32_t matrix_rgb(double r, double g, double b);
void animation_1(PIO *pio, uint *sm);

#endif