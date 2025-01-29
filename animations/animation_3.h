#ifndef ANIMATION_3_H
#define ANIMATION_3_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para inicializar o gerador de números aleatórios

uint32_t matrix_rgb(double r, double g, double b);
void animation_3(PIO *pio, uint *sm);

#endif