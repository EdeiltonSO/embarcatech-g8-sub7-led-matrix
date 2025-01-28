#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para inicializar o gerador de números aleatórios

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
void key1_animation(PIO *pio, uint *sm);
void key2_animation(PIO *pio, uint *sm);
void key3_animation(PIO *pio, uint *sm);
void key4_animation(PIO *pio, uint *sm);
void key5_animation();
void key6_animation();

void all_leds_blue(PIO *pio, uint *sm);
void all_leds_red(PIO *pio, uint *sm);
void all_leds_green(PIO *pio, uint *sm);
void all_leds_white(PIO *pio, uint *sm);
void all_leds_off(PIO *pio, uint *sm);

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
            key1_animation(&pio, &sm);
            break;
        case '2':
            key2_animation(&pio, &sm);
            break;
        case '3':
            key3_animation(&pio, &sm);
            break;
        case '4': // Mostra o nome GUSTAVO
            key4_animation(&pio, &sm);
            break;
        case '5':
            key5_animation();
            break;
        case '6':
            key6_animation();
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

void key1_animation(PIO *pio, uint *sm) {
    const uint FRAMES = 40, FRAME_DIMENSION = 5;

    double animate[40][5][5] = {
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {1.0, 1.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {1.0, 1.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {1.0, 1.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {1.0, 1.0, 0.0, 0.0, 1.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 1.0, 0.0, 0.0, 1.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 1.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 1.0, 0.0, 0.0},
            {0.0, 1.0, 1.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 1.0, 1.0, 0.0, 0.0},
            {0.0, 1.0, 1.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 1.0, 1.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 1.0, 0.0, 1.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 1.0, 1.0, 1.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 1.0, 1.0, 1.0},
            {0.0, 0.0, 1.0, 1.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 1.0, 1.0, 1.0},
            {0.0, 0.0, 1.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 1.0, 1.0, 1.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 1.0, 1.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 1.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 1.0},
            {0.0, 0.0, 0.0, 1.0, 1.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 1.0, 1.0},
            {0.0, 0.0, 0.0, 1.0, 1.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 1.0, 1.0}
        },
        // 27 frames até aqui
        // frames após alcançar a comida
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 1.0, 1.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0, 1.0, 1.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {1.0, 1.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {1.0, 1.0, 1.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {1.0, 1.0, 1.0, 1.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 0.0, 0.0, 0.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {1.0, 1.0, 1.0, 1.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0},
            {1.0, 1.0, 0.0, 1.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
        {
            {1.0, 1.0, 1.0, 1.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0},
            {1.0, 1.0, 1.0, 1.0, 1.0}
        },
    };

    // percorre o objeto animation até passar por todos os frames
    for (int i = 0; i < FRAMES; i++)
    {
        // liga os LEDs conforme cada frame
        for (int j = 0; j < FRAME_DIMENSION; j++)
        {
            for (int k = 0; k < FRAME_DIMENSION; k++)
            {
                if (j == 4 && k == 0 && i < 26) {
                    // se for o led da comida da cobrinha, acender vermelho
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(animate[i][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 0.0, 0.0));
                }
                else if ((j != 4 || k != 0) && i < 26) {
                    // se for parte do corpo da cobrinha, acender verde
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, animate[i][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 0.0));
                }
                else if (i >= 26) {
                    // se a cobrinha alcançar a comida, acender azul
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, 0.0, animate[i][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1]));
                }
            }
        }
        // tempo de duração de cada frame
        sleep_ms(150);
    }
}

//Função específica da 2° animação
void resetTransitionFrame(double matrix[5][5], const uint *FRAME_DIMENSION)
{
    for (int i = 0; i < *FRAME_DIMENSION; i++)
    {
        for (int j = 0; j < *FRAME_DIMENSION; j++)
        {
            matrix[i][j] = 0.0;
        }
    }
}

//Animação de uma contagem de 1 a 9
void key2_animation(PIO *pio, uint *sm) {
    const uint DRAWS = 10, FRAME_DIMENSION = 5;

    double animate[10][5][5] = {

// Matriz responsável por receber os padrões que representarão as transições entre desenhos
        {{0.0, 0.0, 0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0, 0.0, 0.0}},
///////////////////////////////////////////////////////////////////

// Desenhos que serão usados nos padrões de leds
        {{0.0, 0.0, 1.0, 0.0, 0.0},
         {0.0, 1.0, 1.0, 0.0, 0.0},
         {1.0, 0.0, 1.0, 0.0, 0.0},
         {0.0, 0.0, 1.0, 0.0, 0.0},
         {0.0, 0.0, 1.0, 0.0, 0.0}},

        {{1.0, 1.0, 1.0, 1.0, 1.0},
         {1.0, 0.0, 0.0, 1.0, 0.0},
         {0.0, 0.0, 1.0, 0.0, 0.0},
         {0.0, 1.0, 0.0, 0.0, 0.0},
         {1.0, 1.0, 1.0, 1.0, 1.0}},

        {{1.0, 1.0, 1.0, 1.0, 1.0},
         {0.0, 0.0, 0.0, 0.0, 1.0},
         {1.0, 1.0, 1.0, 1.0, 1.0},
         {0.0, 0.0, 0.0, 0.0, 1.0},
         {1.0, 1.0, 1.0, 1.0, 1.0}},

        {{1.0, 0.0, 0.0, 0.0, 1.0},
         {1.0, 0.0, 0.0, 0.0, 1.0},
         {1.0, 1.0, 1.0, 1.0, 1.0},
         {0.0, 0.0, 0.0, 0.0, 1.0},
         {0.0, 0.0, 0.0, 0.0, 1.0}},

        {{1.0, 1.0, 1.0, 1.0, 1.0},
         {1.0, 0.0, 0.0, 0.0, 0.0},
         {1.0, 1.0, 1.0, 1.0, 1.0},
         {0.0, 0.0, 0.0, 0.0, 1.0},
         {1.0, 1.0, 1.0, 1.0, 1.0}},

        {{1.0, 1.0, 1.0, 1.0, 0.0},
         {1.0, 0.0, 0.0, 0.0, 0.0},
         {1.0, 1.0, 1.0, 1.0, 0.0},
         {1.0, 0.0, 0.0, 1.0, 0.0},
         {1.0, 1.0, 1.0, 1.0, 0.0}},

        {{1.0, 1.0, 1.0, 1.0, 0.0},
         {0.0, 0.0, 0.0, 1.0, 0.0},
         {0.0, 0.0, 0.0, 1.0, 0.0},
         {0.0, 0.0, 0.0, 1.0, 0.0},
         {0.0, 0.0, 0.0, 1.0, 0.0}},

        {{1.0, 1.0, 1.0, 1.0, 1.0},
         {1.0, 0.0, 0.0, 0.0, 1.0},
         {1.0, 1.0, 1.0, 1.0, 1.0},
         {1.0, 0.0, 0.0, 0.0, 1.0},
         {1.0, 1.0, 1.0, 1.0, 1.0}},

        {{1.0, 1.0, 1.0, 1.0, 0.0},
         {1.0, 0.0, 0.0, 1.0, 0.0},
         {1.0, 1.0, 1.0, 1.0, 0.0},
         {0.0, 0.0, 0.0, 1.0, 0.0},
         {0.0, 0.0, 0.0, 1.0, 0.0}},
///////////////////////////////////////////////////////////////////
    };

    // Laço responsável por percorrer todos os desenhos
    for (int i = 0; i < DRAWS; i++)
    {
        // Função usada para resetar a matriz de transição entre desenhos
        resetTransitionFrame(animate[0], &FRAME_DIMENSION);

        // Liga os Leds no padrão do desenho representado na matriz
        for (int j = 0; j < FRAME_DIMENSION; j++)
        {
            for (int k = 0; k < FRAME_DIMENSION; k++)
            {
                // Envia as informações de cor e intensidade do led para a maquina de estado
                pio_sm_put_blocking(*pio, *sm, matrix_rgb(animate[i][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 0.0, 0.0));
            }
        }
        // Mantém a matriz de leds com o padrão do desenho por 1 segundo
        sleep_ms(1000);

        // Atualiza os valores da matriz de trasição
        for (int col = FRAME_DIMENSION - 1; col >= 0; col--)
        {
            for (int row = FRAME_DIMENSION - 1; row >= 0; row--)
            {
                animate[0][row][col] = 1.0;
            }

            for (int j = 0; j < FRAME_DIMENSION; j++)
            {
                for (int k = 0; k < FRAME_DIMENSION; k++)
                {
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(animate[0][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 0.0, 0.0));
                }
            }
            //Mantém o padrão de leds aceso por 0.1 segundo antes de mudar para o próximo estado da trasição
            sleep_ms(100);
        }
    }
}

//Animação de jogo de ping pong
void key3_animation(PIO *pio, uint *sm) { 
    const uint FRAME_DIMENSION = 5; // Tamanho da matriz (5x5)
    int game_over = 0;              // Indica se o jogo terminou

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Posições numeradas das "bolinhas azuis"
    int blue_positions[] = {5, 10, 15, 9, 14, 19}; // Esquerda e direita
    int sequence[] = {5, 19, 10, 9, 4};            // Sequência da bolinha vermelha
    int sequence_length = sizeof(sequence) / sizeof(sequence[0]);
    int current_target = 0; // Índice do alvo atual na sequência

    // Posição inicial da bolinha vermelha
    int ball_row = 1; // Começa na posição 5 (linha 1)
    int ball_col = 0; // Começa na coluna 0 (lado esquerdo)

    for (int frame = 0; frame < 30; frame++) { // Total de 30 frames
        if (game_over) {
            // Se o jogo acabou, preenche a tela toda de verde
            for (int row = 0; row < FRAME_DIMENSION; row++) {
                for (int col = 0; col < FRAME_DIMENSION; col++) {
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, 1.0, 0.0)); // Verde
                }
            }
            sleep_ms(1000); // Pausa para visualizar o estado final

            // Desliga todos os LEDs após exibir o verde
            for (int row = 0; row < FRAME_DIMENSION; row++) {
                for (int col = 0; col < FRAME_DIMENSION; col++) {
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, 0.0, 0.0)); // Preto
                }
            }
            break; // Finaliza o loop
        }

        // Atualiza a posição da bolinha vermelha em direção ao alvo
        int target_index = sequence[current_target];
        int target_row = target_index / FRAME_DIMENSION;
        int target_col = target_index % FRAME_DIMENSION;

        if (current_target == sequence_length - 1) {
            // Se o alvo atual for o último (posição 4), a bolinha vai diretamente ao alvo
            ball_row = target_row;
            ball_col = target_col;
            game_over = 1; // Termina o jogo ao atingir o último alvo
        } else {
            // Movimento normal para outros alvos
            if (ball_row < target_row) ball_row++;
            else if (ball_row > target_row) ball_row--;

            if (ball_col < target_col) ball_col++;
            else if (ball_col > target_col) ball_col--;
        }

        // Verifica se a bolinha atingiu o alvo
        if (ball_row == target_row && ball_col == target_col && current_target < sequence_length - 1) {
            current_target++; // Avança para o próximo alvo
            sleep_ms(300);    // Pausa de 300 ms ao atingir o alvo antes de continuar
        }

        // Renderiza o frame
        for (int row = 0; row < FRAME_DIMENSION; row++) {
            for (int col = 0; col < FRAME_DIMENSION; col++) {
                int position_index = row * FRAME_DIMENSION + col;

                if (position_index == 5 || position_index == 10 || position_index == 15 || position_index == 9 || position_index == 14 || position_index == 19) {
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, 0.0, 1.0)); // Azul para as bolinhas fixas
                } else if (row == ball_row && col == ball_col) {
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(1.0, 0.0, 0.0)); // Vermelho para a bolinha
                } else {
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, 0.0, 0.0)); // Preto para o fundo
                }
            }
        }

        sleep_ms(100); // 10 FPS
    }
}

void key4_animation(PIO *pio, uint *sm) { // Animação do nome Gustavo
    const uint FRAMES=37, FRAME_DIMENSION=5;
    
    double animacao[37][5][5]={
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},//normal   ok
            {0.0, 0.0, 0.0, 0.0, 0.0},//inverte
            {0.0, 0.0, 0.5, 0.5, 0.5},//normal
            {0.5, 0.0, 0.0, 0.0, 0.0},//inverte
            {0.0, 0.0, 0.0, 0.0, 0.0},//normal

        },// frame 1
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},//ok
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.5, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.5},

        },// frame 2
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},//ok
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.5, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 3
        {
            {0.5, 0.0, 0.0, 0.0, 0.0},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.5, 0.5, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 4
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.5, 0.5, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 5, fim da letra G
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },// frame 6
         {
            {0.5, 0.0, 0.0, 0.0, 0.0},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},

        },// frame 7
        {
            {0.5, 0.0, 0.0, 0.0, 0.0},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 8
        {
            {0.5, 0.0, 0.0, 0.0, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 9, fim da letra U
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },// frame 10
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },// frame 11
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },// frame 12
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.5},

        },// frame 13
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 14, fim da letra S
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },// frame 15
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},//ok
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},

        },// frame 16
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},

        },// frame 17, fim da letra T
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },
        {
            {0.5, 0.0, 0.0, 0.0, 0.0},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},

        },// frame 19
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},

        },// frame 20
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},

        },// frame 21
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},

        },// frame 22, fim da letra A
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },// frame 23
        {
            {0.5, 0.0, 0.0, 0.0, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },// frame 24
        {
            {0.5, 0.0, 0.0, 0.0, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.0, 0.5, 0.0, 0.5, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },// frame 25
        {
            {0.5, 0.0, 0.0, 0.0, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.0, 0.5, 0.0, 0.5, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},

        },// frame 26, fim da letra V
        {
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0},

        },//27
        {
            {0.5, 0.5, 0.0, 0.0, 0.0},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.0},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.0, 0.0, 0.5},

        },// frame 28
        {
            {0.5, 0.5, 0.0, 0.5, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.0, 0.5, 0.5},

        },// frame 29
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 30, fim da letra O, fim do nome.
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.5, 0.5, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 31, G
        {
            {0.5, 0.0, 0.0, 0.0, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 32, U
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},
            {0.0, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        },// frame 33, S
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},

        },// frame 34, T
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},

        },// frame 35, A
        {
            {0.5, 0.0, 0.0, 0.0, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.0, 0.5, 0.0, 0.5, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},
            {0.0, 0.0, 0.5, 0.0, 0.0},

        },// frame 36, V
        {
            {0.5, 0.5, 0.5, 0.5, 0.5},//ok
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.0, 0.0, 0.0, 0.5},
            {0.5, 0.5, 0.5, 0.5, 0.5},

        }// frame 37, O
    };
    // Laço da animação
    for(uint i=0;i<FRAMES;i++){

        if(i%2==0 && i<31){
            for(uint j=0;j<FRAME_DIMENSION;j++){
                for(uint k=0;k<FRAME_DIMENSION;k++){
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(animacao[i][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 0.0, 0.0));

                }

            }

        }

        else{

            for(uint j=0;j<FRAME_DIMENSION;j++){
                for(uint k=0;k<FRAME_DIMENSION;k++){
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, animacao[i][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 0.0));

                }

            }

        }
        sleep_ms(230);// tempo de cada frame
    }
}

void key5_animation() {
    // implementar animação da tecla 5
}

void key6_animation() {
    // implementar animação da tecla 6
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
