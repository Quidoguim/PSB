/*
 * Nome do arquivo: Exercicio002.c
 * Autor: Guilherme G. Santos
 * Objetivo: Ler um arquivo binário contendo um inteiro de 4 bytes seguido por inteiros de 4 bytes, e imprimir a soma deles.
 * Data: 2026-04-05
 */

#include <stdio.h>
#include <stdint.h>

int main() {
    char filename[20];
    FILE *file;
    int32_t count;
    long long sum = 0;
    int32_t value;

    if (scanf("%19s", filename) == 1) {
        file = fopen(filename, "rb");
        
        if (file == NULL) {
            printf("ARQUIVO INEXISTENTE\n");
            return 0;
        }

        if (fread(&count, sizeof(int32_t), 1, file) == 1) {
            for (int32_t i = 0; i < count; i++) {
                if (fread(&value, sizeof(int32_t), 1, file) == 1) {
                    sum += value;
                }
            }
            
            printf("%lld\n", sum);
        }
        
        fclose(file);
    }
    
    return 0;
}
