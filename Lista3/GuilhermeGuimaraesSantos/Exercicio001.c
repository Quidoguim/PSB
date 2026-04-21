/*
 * Nome do arquivo: Exercicio001.c
 * Autor: Guilherme G. Santos
 * Propósito: Calcular a soma e a multiplicação de duas frações.
 * Data: 2026-04-21
 */

#include <stdio.h>

typedef struct {
    int numerador;
    int denominador;
} fracao_t;

int mmc(int a, int b) {
    int remainder, n1, n2;
    n1 = a;
    n2 = b;
    do {
        remainder = n1 % n2;
        n1 = n2;
        n2 = remainder;
    } while (remainder != 0);
    return (a * b) / n1;
}

fracao_t soma(fracao_t a, fracao_t b) {
    fracao_t result;
    result.denominador = mmc(a.denominador, b.denominador);
    result.numerador = (result.denominador / a.denominador * a.numerador) + 
                       (result.denominador / b.denominador * b.numerador);
    return result;
}

fracao_t multiplica(fracao_t a, fracao_t b) {
    fracao_t result;
    result.numerador = a.numerador * b.numerador;
    result.denominador = a.denominador * b.denominador;
    return result;
}

int main() {
    fracao_t f1, f2, res;

    if (scanf("%d %d", &f1.numerador, &f1.denominador) == 2) {

        if (scanf("%d %d", &f2.numerador, &f2.denominador) == 2) {
            
            res = soma(f1, f2);
            printf("%d %d\n", res.numerador, res.denominador);

            res = multiplica(f1, f2);
            printf("%d %d\n", res.numerador, res.denominador);
        }
    }
    return 0;
}
