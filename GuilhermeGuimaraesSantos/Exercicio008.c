/*
 * Nome do arquivo: Exercicio008.c
 * Autor: Guilherme G. Santos
 * Propósito: Calcular a soma dos fatoriais inversos até n.
 * Data: 2026-03-17
 */

#include <stdio.h>

int main() {
    int n;
    
    if (scanf("%d", &n) == 1) {
        double sum = 0.0;
        double factorial = 1.0;

        for (int i = 1; i <= n; i++) {
            factorial *= i;
            sum += 1.0 / factorial;
        }
        
        printf("%.10lf\n", sum);
    }
    
    return 0;
}