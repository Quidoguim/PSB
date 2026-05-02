/*
 * Nome do arquivo: Exercicio003.c
 * Autor: Guilherme G. Santos
 * Objetivo: Converter intensidades RGB para um valor de paleta fixa de 8 bits.
 * Data: 2026-05-02
 */

#include <stdio.h>

int main() {
    unsigned int r, g, b;
    unsigned char result;

    if (scanf("%u %u %u", &g, &r, &b) == 3) {
        unsigned char g_bits = (g & 0xE0);
        unsigned char r_bits = (r & 0xE0) >> 3;
        unsigned char b_bits = (b & 0xC0) >> 6;

        result = g_bits | r_bits | b_bits;

        printf("%u\n", result);
    }

    return 0;
}
