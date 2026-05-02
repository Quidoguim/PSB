/*
 * Nome do arquivo: Exercicio006.c
 * Autor: Guilherme G. Santos
 * Objetivo: Contar quantos bits 0 estão à direita do bit 1 mais significativo.
 * Data: 2026-05-02
 */

#include <stdio.h>

int main() {
    int num;
    int count_zeros = 0;
    int started = 0;

    if (scanf("%d", &num) == 1) {
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;

            if (bit == 1) {
                started = 1;
            } else if (started == 1) {
                count_zeros++;
            }
        }

        if (num == 0) {
            count_zeros = 0;
        }

        printf("%d\n", count_zeros);
    }

    return 0;
}
