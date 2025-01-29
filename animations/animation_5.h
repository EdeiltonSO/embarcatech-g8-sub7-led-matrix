#ifndef ANIMATION_5_H
#define ANIMATION_5_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

uint32_t matrix_rgb(double r, double g, double b);
void resetTransitionFrame(double matrix[5][5], const uint *FRAME_DIMENSION);
void animation_5(PIO *pio, uint *sm);

#endif