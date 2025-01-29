#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"

#include "led_matrix.pio.h"

#include "animations/animation_1.h"
#include "animations/animation_2.h"
#include "animations/animation_3.h"
#include "animations/animation_4.h"
#include "animations/animation_5.h"
#include "animations/animation_6.h"


#define BUZZER_PIN 21

#define OUT_PIN 10

#define NUM_PIXELS 25

const uint8_t PIN_COLUMNS[] = {5, 4, 3, 2};
const uint8_t PINS_ROWS[] = {9, 8, 7, 6};

const char CONJUNTO_DE_CARACTERES[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'},
};

// Protótipos de funções
void all_leds_blue(PIO *pio, uint *sm);
void all_leds_red(PIO *pio, uint *sm);
void all_leds_green(PIO *pio, uint *sm);
void all_leds_white(PIO *pio, uint *sm);
void all_leds_off(PIO *pio, uint *sm);

void buzzer_init();
void bootsel_mode();

void inicializarTeclado();
char verificarPinosAtivos();
void mapearTeclado(char *caractere, PIO pio, uint sm);

uint32_t matrix_rgb(double r, double g, double b);

int main()
{
    PIO pio = pio0;

    set_sys_clock_khz(128000, false);

    stdio_init_all();

    inicializarTeclado();
    buzzer_init();

    // Configurações da PIO
    uint offset = pio_add_program(pio, &led_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    led_matrix_program_init(pio, sm, offset, OUT_PIN);

    while (true) {
        char c = '\0';
        c = verificarPinosAtivos();
        mapearTeclado(&c, pio, sm);
        sleep_ms(150);
    }
}

void inicializarTeclado() {
    // Inicializa o conjunto de pinos que representa as linhas
    for (int i = 0; i < 4; i++) {
        gpio_init(PINS_ROWS[i]);
        gpio_set_dir(PINS_ROWS[i], GPIO_OUT);
        gpio_put(PINS_ROWS[i], 0);
    }
    
    // Inicializa o conjunto de pinos que representa as colunas
    for (int i = 0; i < 4; i++) {
        gpio_init(PIN_COLUMNS[i]);
        gpio_set_dir(PIN_COLUMNS[i], GPIO_IN);
        gpio_pull_down(PIN_COLUMNS[i]);
    }
}

char verificarPinosAtivos() {
    for (int i = 0; i < 4; i++) {
        gpio_put(PINS_ROWS[i], 1);

        for (int j = 0; j < 4; j++) {
            if (gpio_get(PIN_COLUMNS[j])) {
                gpio_put(PINS_ROWS[i], 0);
                return CONJUNTO_DE_CARACTERES[i][j];
            }
        }
        gpio_put(PINS_ROWS[i], 0);
    }
    return '\0';
}

void mapearTeclado(char *caractere, PIO pio, uint sm) { 
    switch (*caractere) {
        case '1':
            animation_1(&pio, &sm);
            break;
        case '2':
            animation_2(&pio, &sm);
            break;
        case '3':
            animation_3(&pio, &sm);
            break;
        case '4': // Mostra o nome GUSTAVO
            animation_4(&pio, &sm);
            break;
        case '5':
            animation_5(&pio, &sm);
            break;
        case '6':
            animation_6(&pio, &sm, BUZZER_PIN);
            break;
        case 'A':
            all_leds_off(&pio, &sm);
            break;
        case 'B':
            all_leds_blue(&pio, &sm);
            break;
        case 'C':
            all_leds_red(&pio, &sm);
            break;
        case 'D':
            all_leds_green(&pio, &sm);
            break;
        case '#':
            all_leds_white(&pio, &sm);
            break;
        case '*':
            bootsel_mode();
            break;
        default:
            break;
    }
}

void all_leds_blue(PIO *pio,uint *sm) {
    for (int i = NUM_PIXELS; i >= 0; i--)
    {
        pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, 0.0, 1.0));
    }
}

void all_leds_red(PIO *pio, uint *sm) {
    // ligar todos os LEDs da matriz na cor VERMELHA com 80% de intensidade
     for (int i=0; i<NUM_PIXELS; i++){
        pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.8, 0.0, 0.0));
     }
}

void all_leds_green(PIO *pio, uint *sm) {
    // ligar todos os LEDs da matriz na cor VERDE com 50% de intensidade
    for (int i=0; i<NUM_PIXELS; i++){
        pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, 0.5, 0.0));
     }

}

void all_leds_white(PIO *pio, uint *sm) {
    // ligar todos os LEDs da matriz na cor BRANCA com 20% de intensidade
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.2, 0.2, 0.2));
    }
}

void all_leds_off(PIO *pio, uint *sm) {
    // desligar todos os LEDs da matriz
    for(int i=0; i<NUM_PIXELS;i++){
        pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, 0.0, 0.0));
    }
}

void buzzer_init() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);
}

uint32_t matrix_rgb(double r, double g, double b){
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

void bootsel_mode() {
    reset_usb_boot(0, 0);
}
