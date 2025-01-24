#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "blink.pio.h"

#define BUZZER_PIN 21

const uint PIN_COLUMNS[] = {5, 4, 3, 2};
const uint PINS_ROWS[] = {9, 8, 7, 6};

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
void all_leds_white();
void all_leds_off();

void buzzer_init();
void buzzer_on();
void buzzer_off();

void inicializarTeclado();
char verificarPinosAtivos();
void mapearTeclado(char *caractere);

// void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
//     blink_program_init(pio, sm, offset, pin);
//     pio_sm_set_enabled(pio, sm, true);

//     printf("Blinking pin %d at %d Hz\n", pin, freq);

//     // PIO counter program takes 3 more cycles in total than we pass as
//     // input (wait for n + 1; mov; jmp)
//     pio->txf[sm] = (125000000 / (2 * freq)) - 3;
// }

int main()
{
    stdio_init_all();

    inicializarTeclado();
    buzzer_init();

    // // PIO Blinking example
    // PIO pio = pio0;
    // uint offset = pio_add_program(pio, &blink_program);
    // printf("Loaded program at %d\n", offset);
    
    // #ifdef PICO_DEFAULT_LED_PIN
    // blink_pin_forever(pio, 0, offset, PICO_DEFAULT_LED_PIN, 3);
    // #else
    // blink_pin_forever(pio, 0, offset, 11, 3);
    // #endif
    // // For more pio examples see https://github.com/raspberrypi/pico-examples/tree/master/pio

    while (true) {
        char c = '\0';
        c = verificarPinosAtivos();
        mapearTeclado(&c);
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

void mapearTeclado(char *caractere) { 
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
            buzzer_off();
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
            all_leds_white();
            break;
        case '*':
            // modo bootsel
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
void all_leds_white() {
    // ligar todos os LEDs da matriz na cor BRANCA com 20% de intensidade
}
void all_leds_off() {
    // desligar todos os LEDs da matriz
}

void buzzer_init() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);
}
void buzzer_on() {
    // implementar som do buzzer
}
void buzzer_off() {
    // desligar buzzer
}