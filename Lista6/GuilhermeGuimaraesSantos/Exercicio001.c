/*
 * Nome do arquivo: Exercicio001.c
 * Autor: Guilherme G. Santos
 * Objetivo: Gerar um Diagrama de Voronoi usando uma matriz de caracteres, atribuindo a cada célula o número da semente mais próxima com base na distância Euclidiana.
 * Data: 2026-05-19
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_SEEDS 5

int main() {
    int rows, cols;

    if (scanf("%d %d", &rows, &cols) != 2) return 0;

    int seed_row[NUM_SEEDS], seed_col[NUM_SEEDS];
    for (int i = 0; i < NUM_SEEDS; i++) {
        if (scanf("%d %d", &seed_row[i], &seed_col[i]) != 2) return 0;
    }

    char **matrix = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (char *)malloc(cols * sizeof(char));
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = ' ';
        }
    }

    for (int k = 0; k < NUM_SEEDS; k++) {
        matrix[seed_row[k]][seed_col[k]] = '.';
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == ' ') {
                int nearest = 0;
                double min_dist = -1.0;

                for (int k = 0; k < NUM_SEEDS; k++) {
                    double di = (double)(i - seed_row[k]);
                    double dj = (double)(j - seed_col[k]);
                    double dist = sqrt(di * di + dj * dj);

                    if (min_dist < 0.0 || dist < min_dist) {
                        min_dist = dist;
                        nearest = k;
                    }
                }

                matrix[i][j] = '1' + nearest;
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
