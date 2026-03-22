/*
 * File Name: Exercicio005.c
 * Author: Guilherme G. Santos
 * Purpose: Calcula as raízes de uma equação quadrática usando a fórmula de Bhaskara.
 * Date: 2026-03-22
 */

#include <stdio.h>
#include <math.h>

int bhaskara(double a, double b, double c, double *root1, double *root2) {
    double delta = (b * b) - (4.0 * a * c);

    if (delta < 0.0) {
        return 0;
    } else if (delta == 0.0) {
        *root1 = -b / (2.0 * a);
        return 1;
    } else {
        *root1 = (-b - sqrt(delta)) / (2.0 * a);
        *root2 = (-b + sqrt(delta)) / (2.0 * a);
        return 2;
    }
}

int main() {
    double a, b, c, root1, root2;
    int num_roots;
    
    if (scanf("%lf %lf %lf", &a, &b, &c) == 3) {
        num_roots = bhaskara(a, b, c, &root1, &root2);

        if (num_roots == 0) {
            printf("NENHUMA raiz real\n");
        } else if (num_roots == 1) {
            printf("UMA raiz real: %.4lf\n", root1);
        } else {
            if (root1 > root2) {
                double temp = root1;
                root1 = root2;
                root2 = temp;
            }
            printf("DUAS raizes reais: %.4lf %.4lf\n", root1, root2);
        }
    }

    return 0;
}
