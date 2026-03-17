/*
 * Nome do arquivo: Exercicio002.c
 * Autor: Guilherme G. Santos
 * Propósito: Converter uma temperatura dada de Fahrenheit para Celsius.
 * Data: 2026-03-17
 */

#include <stdio.h>

int main() {
    double fahrenheit, celsius;
    
    if (scanf("%lf", &fahrenheit) == 1) {
        celsius = (5.0 / 9.0) * (fahrenheit - 32.0);

        printf("%.4lf\n", celsius);
    }
    
    return 0;
}