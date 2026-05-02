/*
 * Nome do arquivo: Exercicio005.c
 * Autor: Guilherme G. Santos
 * Objetivo: Desenhar um caractere 8x8 baseado em um array de 8 bytes.
 * Data: 2026-05-02
 */

#include <stdio.h>

int main() {
    unsigned int bytes[8];

    if (scanf("%u %u %u %u %u %u %u %u", 
              &bytes[0], &bytes[1], &bytes[2], &bytes[3], 
              &bytes[4], &bytes[5], &bytes[6], &bytes[7]) == 8) {
        
        for (int i = 0; i < 8; i++) {
            for (int j = 7; j >= 0; j--) {
                if ((bytes[i] >> j) & 1) {
                    printf("x");
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
    }

    return 0;
}
