#ifndef ANIMATION_4_H
#define ANIMATION_4_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

uint32_t matrix_rgb(double r, double g, double b);
void animation_4(PIO *pio, uint *sm);

#endif