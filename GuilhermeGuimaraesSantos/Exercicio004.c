/*
 * Nome do arquivo: Exercicio004.c
 * Autor: Guilherme G. Santos
 * Propósito: Calcular o total de segundos restantes até o final do dia.
 * Data: 2026-03-17
 */

#include <stdio.h>

int main() {
    int hours, minutes, seconds;
    long remaining_seconds;
    
    if (scanf("%d %d %d", &hours, &minutes, &seconds) == 3) {
        long elapsed = (hours * 3600L) + (minutes * 60L) + seconds;
        remaining_seconds = 86400L - elapsed;
        
        printf("%ld\n", remaining_seconds);
    }
    
    return 0;
}