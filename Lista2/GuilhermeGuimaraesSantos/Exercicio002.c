/*
 * File Name: Exercicio002.c
 * Author: Guilherme G. Santos
 * Purpose: Converte segundos em horas, minutos e segundos.
 * Date: 2026-03-22
 */

#include <stdio.h>

void hms(unsigned int total_seconds, unsigned int *hours, unsigned int *minutes, unsigned int *seconds) {
    *hours = total_seconds / 3600;
    total_seconds %= 3600;
    *minutes = total_seconds / 60;
    *seconds = total_seconds % 60;
}

int main() {
    unsigned int total_seconds, hours, minutes, seconds;
    
    if (scanf("%u", &total_seconds) == 1) {
        hms(total_seconds, &hours, &minutes, &seconds);

        printf("%u %u %u\n", hours, minutes, seconds);
    }

    return 0;
}
