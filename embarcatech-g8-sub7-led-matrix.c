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
void key1_animation(PIO *pio, uint *sm);
void key2_animation(PIO *pio, uint *sm);
void key3_animation();
void key4_animation();
void key5_animation(PIO *pio, uint *sm);
void key6_animation();

void all_leds_blue(PIO *pio, uint *sm);
void all_leds_red();
void all_leds_green();
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
            key3_animation();
            break;
        case '4':
            key4_animation();
            break;
        case '5':
            key5_animation(&pio, &sm);
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
            all_leds_red();
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
                animate[0][row][col] = 0.5;
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

void key3_animation() {
    // implementar animação da tecla 3
}

void key4_animation() {
    // implementar animação da tecla
}

void key5_animation(PIO *pio, uint *sm) {
    // implementar animação da tecla 5
    const uint FRAMES =35, FRAME_DIMENSION = 5;

    double animate [35][5][5]={
    //Matriz zero - recebe os padr�es para transi��o 
    {{0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    //Matriz de ilustra��es contendo a abrevia��o do nome "Junior", seguida por um desenho de um rosto como destaque final.
    {{0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{0.0, 0.0, 0.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{0.0, 0.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{0.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0., 1.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 1.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 1.0, 1.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 1.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 1.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 1.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 0.0, 0.0, 1.0},
      {1.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 1.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 1.0, 0.0, 1.0},
      {1.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 1.0, 1.0, 1.0}},
//17
    {{0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0., 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 1.0}},

    {{0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0., 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0., 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0., 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0., 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0., 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{0.0, 0.0, 1.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0., 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{0.0, 1.0, 1.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0., 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0., 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 0.0},
      {1.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0., 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 0.0},
      {1.0, 0.0, 0.0, 1.0, 0.0},
      {1.0, 0., 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 0.0},
      {1.0, 0.0, 0.0, 1.0, 0.0},
      {1.0, 0., 0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0}},

    {{1.0, 1.0, 1.0, 1.0, 0.0},
      {1.0, 0.0, 0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0, 1.0, 1.0}},

    {{0.5, 0.5, 0.0, 0.5, 0.5},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 1.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0}},

    {{0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.5, 0.0, 0.0, 0.0, 0.5},
      {0.5, 0.5, 0.5, 0.5, 0.5}}, 
    
    {{1.0, 1.0, 0.0, 1.0, 1.0},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 1.0, 0.0, 0.0},
      {1.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 1.0, 1.0, 1.0}},
    
    };

    for (int i = 0; i<FRAMES; i++)
    {
        resetTransitionFrame(animate[0], &FRAME_DIMENSION);

        for (int j = 0; j < FRAME_DIMENSION; j++)
        {
            for (int k = 0; k < FRAME_DIMENSION; k++)
            {
                //cor do j em verde
                if( i <=17){

                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, animate[i][FRAME_DIMENSION - 1 -j ][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 0.0));
                // cor do r em azul
                }else if(i <=30){

                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, 0.0, animate[i][FRAME_DIMENSION - 1 -j ][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1]));
                }else{

                    pio_sm_put_blocking(*pio, *sm, matrix_rgb(0.0, animate[i][FRAME_DIMENSION - 1 -j ][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 0.0));
                }
            
            }
        }
        sleep_ms(1000);

        for (int column = FRAME_DIMENSION - 1; column >= 0; column--){
            for (int row = FRAME_DIMENSION - 1; row >= 0; row--)
            {
                animate[0][row][column] = 0.5;
            }
        
        for (int j = 0; j < FRAME_DIMENSION; j++)
            {
                for (int k = 0; k < FRAME_DIMENSION; k++)
                {
                    pio_sm_put_blocking(*pio, *sm, matrix_rgb (animate[0][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], animate[0][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], animate[0][FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1]));
                void all_leds_off();
                }
               
            }
            sleep_ms(100);
        }
    
    } 

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

void all_leds_red() {
    // ligar todos os LEDs da matriz na cor VERMELHA com 80% de intensidade
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