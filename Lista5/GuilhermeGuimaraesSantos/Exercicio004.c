/*
 * Nome do arquivo: Exercicio004.c
 * Autor: Guilherme G. Santos
 * Objetivo: Converter um valor de paleta fixa de 8 bits de volta para intensidades RGB.
 * Data: 2026-05-02
 */

#include <stdio.h>

int main() {
    unsigned int color;
    unsigned int r, g, b;

    if (scanf("%u", &color) == 1) {
        g = color & 0xE0;
        r = (color & 0x1C) << 3;
        b = (color & 0x03) << 6;

        printf("%u %u %u\n", g, r, b);
    }

    return 0;
}
