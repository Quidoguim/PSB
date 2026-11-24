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

## Parte 4 — Mais código, no estilo das listas da professora

Os exercícios abaixo seguem o que foi pedido nas listas e fixações do Moodle durante o semestre. Escreva **em C de verdade** (não pseudocódigo). Ao lado de cada um está o assunto.

**Básico e funções (Lista 1)**

17. Programa completo: ler 10 inteiros e mostrar quantos são positivos, quantos são negativos e quantos são zero.
18. `float celsius(float f)` — converte Fahrenheit para Celsius: C = 5/9 × (F − 32). Cuidado com a divisão.
19. `int primo(int n)` — devolve 1 se n é primo e 0 se não é.
20. `void horas(int total, int *h, int *m, int *s)` — recebe segundos e devolve, pelos ponteiros, horas, minutos e segundos. Ex.: 3725 → 1 h, 2 min, 5 s. Mostre também a chamada no `main`.

**Strings (fixação de strings)**

21. `int vogais(char s[])` — conta as vogais minúsculas de s.
22. `void maiusculas(char s[])` — troca cada letra minúscula pela maiúscula ("casa" → "CASA"). Dica: `'a' - 'A'` vale 32.

**Structs (fixação de structs)**

23. Declare o tipo `Fracao` (numerador e denominador) e escreva `Fracao soma(Fracao a, Fracao b)`. Ex.: 1/2 + 1/3 = 5/6.
24. Com `typedef struct { char nome[30]; float nota; } Aluno;`, escreva `void melhor(Aluno v[], int n)`, que mostra o nome do aluno de maior nota.
25. `void bonus(Aluno *a, float b)` — soma b à nota do aluno, sem passar de 10. Use `->`.

**Ponteiros (fixação de ponteiros)**

26. `int soma(int *v, int n)` — soma os elementos **sem usar `[]`**.
27. `void inverte(int *v, int n)` — inverte o vetor com dois ponteiros, um no início e outro no fim.

**Arquivos (fixação de arquivos)**

28. `void salva(int v[], int n)` grava os n números em `numeros.txt`, um por linha. `int somaArquivo()` lê o arquivo e devolve a soma.

**Bits (fixação de bits)**

29. `setbit(x, b)` liga o bit b de x; `clearbit(x, b)` desliga; `testBit(x, b)` devolve 0 ou 1. Use `unsigned int`.

**Alocação dinâmica e lista (aula de alocação dinâmica e tarefa da lista)**

30. `int *copia(int v[], int n)` — cria com `malloc` um vetor novo, copia v para ele e devolve. Quem chama faz o `free`.
31. Com as structs da tarefa da lista encadeada (abaixo), escreva `void add(List *l, int val)`, que insere no **fim**, e `int somaLista(List *l)`.
    ```c
    typedef struct TNode {
        int data;
        struct TNode *next;
    } Node;

    typedef struct {
        Node *head;
        Node *tail;
        int size;
    } List;
    ```

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

**Parte 4**

17.
```c
#include <stdio.h>

int main() {
    int x, pos = 0, neg = 0, zer = 0;
    for (int i = 0; i < 10; i++) {
        scanf("%d", &x);
        if (x > 0)
            pos++;
        else if (x < 0)
            neg++;
        else
            zer++;
    }
    printf("%d positivos, %d negativos, %d zeros\n", pos, neg, zer);
    return 0;
}
```

18.
```c
float celsius(float f) {
    return 5.0 / 9 * (f - 32);    // 5 / 9 daria 0!
}
```

19.
```c
int primo(int n) {
    if (n < 2)
        return 0;
    for (int i = 2; i < n; i++)
        if (n % i == 0)
            return 0;
    return 1;
}
```

20.
```c
void horas(int total, int *h, int *m, int *s) {
    *h = total / 3600;
    *m = total % 3600 / 60;
    *s = total % 60;
}

int main() {
    int h, m, s;
    horas(3725, &h, &m, &s);
    printf("%d h %d min %d s\n", h, m, s);   // 1 h 2 min 5 s
    return 0;
}
```

21.
```c
int vogais(char s[]) {
    int cont = 0;
    for (int i = 0; s[i] != '\0'; i++)
        if (s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u')
            cont++;
    return cont;
}
```

22.
```c
void maiusculas(char s[]) {
    for (int i = 0; s[i] != '\0'; i++)
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] = s[i] - 32;
}
```

23.
```c
typedef struct {
    int num;
    int den;
} Fracao;

Fracao soma(Fracao a, Fracao b) {
    Fracao r;
    r.num = a.num * b.den + b.num * a.den;
    r.den = a.den * b.den;
    return r;
}
```

24.
```c
void melhor(Aluno v[], int n) {
    int m = 0;                      // posição do melhor até agora
    for (int i = 1; i < n; i++)
        if (v[i].nota > v[m].nota)
            m = i;
    printf("%s\n", v[m].nome);
}
```

25.
```c
void bonus(Aluno *a, float b) {
    a->nota = a->nota + b;
    if (a->nota > 10)
        a->nota = 10;
}
// chamada: bonus(&al, 1.5);
```

26.
```c
int soma(int *v, int n) {
    int s = 0;
    for (int *p = v; p < v + n; p++)
        s = s + *p;
    return s;
}
```

27.
```c
void inverte(int *v, int n) {
    int *ini = v;
    int *fim = v + n - 1;
    while (ini < fim) {
        int aux = *ini;
        *ini = *fim;
        *fim = aux;
        ini++;
        fim--;
    }
}
```

28.
```c
void salva(int v[], int n) {
    FILE *fp = fopen("numeros.txt", "w");
    if (fp == NULL)
        return;
    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", v[i]);
    fclose(fp);
}

int somaArquivo() {
    FILE *fp = fopen("numeros.txt", "r");
    if (fp == NULL)
        return 0;
    int x, s = 0;
    while (fscanf(fp, "%d", &x) == 1)    // 1 = conseguiu ler um número
        s = s + x;
    fclose(fp);
    return s;
}
```

29.
```c
unsigned int setbit(unsigned int x, int b) {
    return x | (1u << b);
}

unsigned int clearbit(unsigned int x, int b) {
    return x & ~(1u << b);
}

int testBit(unsigned int x, int b) {
    return (x >> b) & 1;
}
// setbit(0, 3) = 8   clearbit(15, 0) = 14   testBit(5, 2) = 1
```

30.
```c
int *copia(int v[], int n) {
    int *novo = malloc(n * sizeof(int));
    if (novo == NULL)
        return NULL;
    for (int i = 0; i < n; i++)
        novo[i] = v[i];
    return novo;
}
// int *c = copia(v, 5);  ...  free(c);
```

31.
```c
void add(List *l, int val) {
    Node *aux = malloc(sizeof(Node));
    aux->data = val;
    aux->next = NULL;
    if (l->tail != NULL)
        l->tail->next = aux;     // o último antigo aponta para o novo
    else
        l->head = aux;           // lista vazia: o novo também é o primeiro
    l->tail = aux;
    l->size++;
}

int somaLista(List *l) {
    int s = 0;
    for (Node *p = l->head; p != NULL; p = p->next)
        s = s + p->data;
    return s;
}
```

Quer mais treino? O material completo está em [Estudo/Prova/](../Prova/README.md): 48 V/F (arquivo 10), 35 "o que imprime" (11) e 15 exercícios (12).
