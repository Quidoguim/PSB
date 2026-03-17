/*
 * Nome do arquivo: Exercicio007.c
 * Autor: Guilherme G. Santos
 * Propósito: Determinar o número mínimo de notas para um valor dado.
 * Data: 2026-03-17
 */

#include <stdio.h>

int main() {
    int amount;
    int notes_100, notes_50, notes_10, notes_5, notes_1;

    if (scanf("%d", &amount) == 1) {
        notes_100 = amount / 100;
        amount %= 100;

        notes_50 = amount / 50;
        amount %= 50;

        notes_10 = amount / 10;
        amount %= 10;

        notes_5 = amount / 5;
        amount %= 5;

        notes_1 = amount;

        printf("%d %d %d %d %d\n", notes_100, notes_50, notes_10, notes_5, notes_1);
    }
    
    return 0;
}