#ifndef ANIMATION_6_H
#define ANIMATION_6_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#define NUM_PIXELS 25
#define BUZZER_PIN 21

uint32_t matrix_rgb(double r, double g, double b);
void animation_6(PIO *pio, uint *sm, uint buzzer_pin);

#endif