/*
 * Nome do arquivo: Exercicio002.c
 * Autor: Guilherme G. Santos
 * Propósito: Verificar se uma string é substring de outra e retornar sua posição.
 * Data: 2026-03-29
 */

#include <stdio.h>
#include <string.h>

#define TAM 100

int eh_substring(char s1[], char s2[]) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    if (len2 == 0) return 0;

    for (int i = 0; i <= len1 - len2; i++) {
        int j;

        for (j = 0; j < len2; j++) {
            if (s1[i + j] != s2[j]) {
                break;
            }
        }
        if (j == len2) {
            return i;
        }
    }

    return -1;
}

int main() {
    int length;
    char str1[TAM + 1], str2[TAM + 1];

    if (fgets(str1, TAM + 1, stdin) != NULL) {
        length = strlen(str1);
        if (str1[length - 1] == '\n') str1[length - 1] = '\0';

        if (fgets(str2, TAM + 1, stdin) != NULL) {
            length = strlen(str2);
            if (str2[length - 1] == '\n') str2[length - 1] = '\0';

            printf("%d\n", eh_substring(str1, str2));
        }
    }

    return 0;
}
