/*
 * Nome do arquivo: Exercicio002.c
 * Autor: Guilherme G. Santos
 * Objetivo: Inverter todos os bits de um inteiro e exibir o resultado.
 * Data: 2026-05-02
 */

#include <stdio.h>

int main() {
    int num, inverted;

    if (scanf("%d", &num) == 1) {
        inverted = ~num;
        printf("%d\n", inverted);
    }

    return 0;
}
