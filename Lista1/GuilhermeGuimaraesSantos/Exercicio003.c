/*
 * Nome do arquivo: Exercicio003.c
 * Autor: Guilherme G. Santos
 * Propósito: Calcular o total de segundos decorridos desde o início do dia.
 * Data: 2026-03-17
 */

#include <stdio.h>

int main() {
    int hours, minutes, seconds;
    long total_seconds;
    
    if (scanf("%d %d %d", &hours, &minutes, &seconds) == 3) {
        total_seconds = (hours * 3600L) + (minutes * 60L) + seconds;
        
        printf("%ld\n", total_seconds);
    }
    
    return 0;
}