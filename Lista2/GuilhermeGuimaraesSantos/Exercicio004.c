/*
 * File Name: Exercicio004.c
 * Author: Guilherme G. Santos
 * Purpose: Calcula a quantidade mínima de moedas para um dado valor de troco.
 * Date: 2026-03-22
 */

#include <stdio.h>

void quantidadeDeMoedas(unsigned int amount, unsigned int *c100, unsigned int *c50, unsigned int *c25, unsigned int *c10, unsigned int *c5, unsigned int *c1) {
    *c100 = amount / 100;
    amount %= 100;

    *c50 = amount / 50;
    amount %= 50;

    *c25 = amount / 25;
    amount %= 25;

    *c10 = amount / 10;
    amount %= 10;

    *c5 = amount / 5;
    amount %= 5;

    *c1 = amount;
}

int main() {
    unsigned int amount, c100, c50, c25, c10, c5, c1;
    
    if (scanf("%u", &amount) == 1) {
        quantidadeDeMoedas(amount, &c100, &c50, &c25, &c10, &c5, &c1);

        printf("%u %u %u %u %u %u\n", c100, c50, c25, c10, c5, c1);
    }

    return 0;
}
