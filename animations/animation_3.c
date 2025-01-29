#include "animation_3.h"

//Animação de uma contagem de 1 a 9
void animation_3(PIO *pio, uint *sm) {
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
