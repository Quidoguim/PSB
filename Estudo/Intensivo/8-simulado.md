# Lição 8 — Simulado

Faça sem consultar, no papel, como na prova. Depois corrija com o gabarito e refaça o que errou.

## Parte 1 — Verdadeiro ou falso (justifique as falsas)

1. `int x; printf("%d", x);` sempre imprime 0.
2. `10 % 3` vale 1.
3. Um `for (i = 0; i < n; i++)` percorre um vetor de n elementos do primeiro ao último.
4. Uma função recebe uma cópia do vetor passado como parâmetro.
5. `char s[] = "abc";` ocupa 4 bytes.
6. `if (strcmp(a, b))` é verdadeiro quando a e b são iguais.
7. Se `p` aponta para `x`, então `*p = 0;` zera `x`.
8. Toda memória pedida com `malloc` deve ser liberada com `free`.
9. O modo `"a"` do `fopen` acrescenta no fim do arquivo.
10. O arquivo `.c` contém só os protótipos das funções.

## Parte 2 — O que imprime?

```c
// 11
int a = 7, b = 2;
float c = a / b;
printf("%.1f\n", c);
```

```c
// 12
int cont = 0;
for (int i = 0; i < 10; i++)
    if (i % 3 == 0)
        cont++;
printf("%d\n", cont);
```

```c
// 13
void f(int a, int *b) {
    a = a + 1;
    *b = *b + 1;
}
// no main:
int x = 1, y = 1;
f(x, &y);
printf("%d %d\n", x, y);
```

## Parte 3 — Escreva o código

14. `int somaPares(int v[], int n)` — devolve a soma só dos elementos pares.
15. `int conta(char s[], char c)` — devolve quantas vezes o caractere `c` aparece em `s`.
16. `void minMax(int v[], int n, int *menor, int *maior)` — devolve, pelos ponteiros, o menor e o maior elemento.

---

## Gabarito

**Parte 1**

| # | | Justificativa |
|---|---|---|
| 1 | F | x não foi inicializado: tem lixo. |
| 2 | V | 10 = 3 × 3 + **1**. |
| 3 | V | i vai de 0 a n−1. |
| 4 | F | Vetor vai pelo endereço; a função mexe no original. |
| 5 | V | 3 letras + `'\0'`. |
| 6 | F | `strcmp` devolve 0 quando iguais, e 0 é falso. Certo: `if (strcmp(a, b) == 0)`. |
| 7 | V | |
| 8 | V | Senão, vazamento de memória. |
| 9 | V | |
| 10 | F | Protótipos ficam no `.h`; o `.c` tem a implementação. |

**Parte 2**

| # | Imprime | Por quê |
|---|---|---|
| 11 | `3.0` | `a / b` é int / int = 3; só depois vira float. |
| 12 | `4` | i = 0, 3, 6, 9. |
| 13 | `1 2` | x foi por cópia; y foi por endereço. |

**Parte 3**

```c
int somaPares(int v[], int n) {
    int s = 0;
    for (int i = 0; i < n; i++)
        if (v[i] % 2 == 0)
            s = s + v[i];
    return s;
}
```

```c
int conta(char s[], char c) {
    int cont = 0;
    for (int i = 0; s[i] != '\0'; i++)
        if (s[i] == c)
            cont++;
    return cont;
}
```

```c
void minMax(int v[], int n, int *menor, int *maior) {
    *menor = v[0];
    *maior = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] < *menor) *menor = v[i];
        if (v[i] > *maior) *maior = v[i];
    }
}
```

Quer mais treino? O material completo está em [Estudo/Prova/](../Prova/README.md): 48 V/F (arquivo 10), 35 "o que imprime" (11) e 15 exercícios (12).
