#include "animation_2.h"

static double animate[10][5][5] = {

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
};

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
void animation_2(PIO *pio, uint *sm) {
    const uint DRAWS = 10, FRAME_DIMENSION = 5;

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
