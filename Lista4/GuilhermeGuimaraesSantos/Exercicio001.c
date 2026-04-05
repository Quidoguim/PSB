/*
 * Nome do arquivo: Exercicio001.c
 * Autor: Guilherme G. Santos
 * Objetivo: Ler um arquivo de texto contendo um número inteiro seguido por essa quantidade de inteiros, e imprimir a soma deles.
 * Data: 2026-04-05
 */

#include <stdio.h>

int main() {
    char filename[20];
    FILE *file;
    int count;
    long long sum = 0;
    long long value;
    
    if (scanf("%19s", filename) == 1) {
        file = fopen(filename, "r");
        
        if (file == NULL) {
            printf("ARQUIVO INEXISTENTE\n");
            return 0;
        }

        if (fscanf(file, "%d", &count) == 1) {
            for (int i = 0; i < count; i++) {
                if (fscanf(file, "%lld", &value) == 1) {
                    sum += value;
                }
            }
            
            printf("%lld\n", sum);
        }
        
        fclose(file);
    }
    
    return 0;
}
