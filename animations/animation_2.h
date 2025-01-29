#ifndef ANIMATION_2_H
#define ANIMATION_2_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

typedef struct {
    double frames[10][5][5];
} Animation_2_Frames;

uint32_t matrix_rgb(double r, double g, double b);
void resetTransitionFrame(double matrix[5][5], const uint *FRAME_DIMENSION);
void animation_2(PIO *pio, uint *sm);

#endif