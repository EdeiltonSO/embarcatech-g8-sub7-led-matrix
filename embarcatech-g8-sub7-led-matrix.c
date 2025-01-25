#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"

#include "blink.pio.h"

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

// Protótipos das funções
void key1_animation();
void key2_animation();
void key3_animation();
void key4_animation();
void key5_animation();
void key6_animation();

void all_leds_blue();
void all_leds_red();
void all_leds_green();
void all_leds_white(PIO pio, uint sm);
void all_leds_off();

void buzzer_init();
void play_buzzer(uint32_t frequency, uint32_t duration_ms);
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
    uint offset = pio_add_program(pio, &blink_program);
    uint sm = pio_claim_unused_sm(pio, true);
    blink_program_init(pio, sm, offset, OUT_PIN);

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
            key1_animation();
            break;
        case '2':
            key2_animation();
            break;
        case '3':
            key3_animation();
            break;
        case '4':
            key4_animation();
            break;
        case '5':
            key5_animation();
            break;
        case '6':
            key6_animation();
            break;
        case 'A':
            all_leds_off();
            break;
        case 'B':
            all_leds_blue();
            break;
        case 'C':
            all_leds_red();
            break;
        case 'D':
            all_leds_green();
            break;
        case '#':
            all_leds_white(pio, sm);
            break;
        case '*':
            bootsel_mode();
            break;
        default:
            break;
    }
}

void key1_animation() {
    // implementar animação da tecla 1
}
void key2_animation() {
    // implementar animação da tecla 2
}
void key3_animation() {
    // implementar animação da tecla 3
}
void key4_animation() {
    // implementar animação da tecla 4
}
void key5_animation() {
    // implementar animação da tecla 5
}
void key6_animation() {
    // implementar animação da tecla 6
}

void all_leds_blue() {
    // ligar todos os LEDs da matriz na cor AZUL com 100% de intensidade
}
void all_leds_red() {
    // ligar todos os LEDs da matriz na cor VERMELHA com 80% de intensidade
}
void all_leds_green() {
    // ligar todos os LEDs da matriz na cor VERDE com 50% de intensidade
}
void all_leds_white(PIO pio, uint sm) {
    // ligar todos os LEDs da matriz na cor BRANCA com 20% de intensidade
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, matrix_rgb(0.2, 0.2, 0.2));
    }
}
void all_leds_off() {
    // desligar todos os LEDs da matriz
}

void buzzer_init() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);
}

void play_buzzer(uint32_t frequency, uint32_t duration_ms) {
    // implementar som do buzzer na frequência frequency e durante duration_ms milissegundos
}

uint32_t matrix_rgb(double r, double g, double b){
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

void bootsel_mode() {
    printf("*** Entrando em modo BOOTSEL ***");
    reset_usb_boot(0, 0);
}