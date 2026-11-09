# 12 — Exercícios de codificação (das listas do Moodle)

Tente escrever no caderno primeiro. As soluções estão logo abaixo de cada um, curtas e no estilo da aula. Todas foram compiladas e testadas.

## 1. Fahrenheit → Celsius (Lista 1)

```c
float celsius(float f) {
    return 5.0 / 9 * (f - 32);   // 5.0: evita a divisão inteira
}
```

## 2. Segundos desde o início do dia (Lista 1)

```c
int segundos(int h, int m, int s) {
    return h * 3600 + m * 60 + s;
}
```

## 3. Primo (Lista 1)

```c
int primo(int n) {
    if (n < 2) return 0;
    for (int i = 2; i < n; i++)
        if (n % i == 0) return 0;
    return 1;
}
```

## 4. Caixa eletrônico, retorno por referência (Lista 1)

```c
void notas(int r, int *n100, int *n50, int *n10, int *n5, int *n1) {
    *n100 = r / 100;  r = r % 100;
    *n50  = r / 50;   r = r % 50;
    *n10  = r / 10;   r = r % 10;
    *n5   = r / 5;
    *n1   = r % 5;
}
// chamada: notas(186, &a, &b, &c, &d, &e);  → 1, 1, 3, 1, 1
```

## 5. Maior e menor de um vetor (atividade 25/08)

```c
void maiorMenor(int v[], int n, int *maior, int *menor) {
    *maior = *menor = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] > *maior) *maior = v[i];
        if (v[i] < *menor) *menor = v[i];
    }
}
```

## 6. Quociente e resto (atividade 25/08)

```c
void divide(int a, int b, int *q, int *r) {
    *q = a / b;
    *r = a % b;
}
```

## 7. Substring (fixação de strings)

Devolve a posição onde `s2` começa dentro de `s1`, ou -1.

```c
int minha_substring(char s1[], char s2[]) {
    for (int i = 0; s1[i] != '\0'; i++) {
        int j = 0;
        while (s2[j] != '\0' && s1[i + j] == s2[j])
            j++;
        if (s2[j] == '\0') return i;
    }
    return -1;
}
// minha_substring("programa", "gra") → 3
```

## 8. Soma de frações com struct (fixação de structs)

```c
typedef struct { int num, den; } Fracao;

Fracao soma(Fracao a, Fracao b) {
    Fracao r;
    r.num = a.num * b.den + b.num * a.den;
    r.den = a.den * b.den;
    return r;
}
// 1/2 + 1/3 = 5/6
```

## 9. Busca em vetor só com ponteiros (fixação de ponteiros)

```c
int busca(int *v, int n, int x) {
    for (int *p = v; p < v + n; p++)
        if (*p == x) return p - v;
    return -1;
}
```

## 10. Inverter vetor só com ponteiros (fixação de ponteiros)

```c
void inverte(int *v, int n) {
    int *ini = v, *fim = v + n - 1;
    while (ini < fim) {
        int t = *ini; *ini = *fim; *fim = t;
        ini++; fim--;
    }
}
```

## 11. Escrever string e número num arquivo texto (fixação de arquivos)

```c
FILE *fp = fopen("saida.txt", "w");
if (fp == NULL) return 1;
fprintf(fp, "%s %d\n", nome, idade);
fclose(fp);
```

## 12. Salvar e carregar structs em binário (fixação de arquivos)

```c
typedef struct { char nome[64]; int idade; float nota; } aluno_t;

int aluno_salva(const char *arq, aluno_t *a) {
    FILE *fp = fopen(arq, "ab");
    if (fp == NULL) return 0;
    fwrite(a, sizeof(aluno_t), 1, fp);
    fclose(fp);
    return 1;
}

int aluno_carrega(const char *arq, aluno_t v[], int len) {
    FILE *fp = fopen(arq, "rb");
    if (fp == NULL) return 0;
    int n = 0;
    while (n < len && fread(&v[n], sizeof(aluno_t), 1, fp) == 1)
        n++;
    fclose(fp);
    return n;
}
```

Repare: em `aluno_salva`, `a` já é ponteiro, então vai **sem** `&` no `fwrite`.

## 13. Bits (fixação de bits)

Ver as 5 funções no resumo [06](06-manipulacao-de-bits.md). Teste rápido: `setbit(0, 3)` = 8; `clearbit(0xFF, 0)` = 254; `testBit(5, 2)` = 1.

## 14. Vetor dinâmico

Ler n, alocar, ler os n valores, mostrar a soma, liberar.

```c
int n, soma = 0;
scanf("%d", &n);
int *v = malloc(n * sizeof(int));
if (v == NULL) return 1;
for (int i = 0; i < n; i++) { scanf("%d", &v[i]); soma += v[i]; }
printf("%d\n", soma);
free(v);
```

## 15. Remover da lista por valor (lista encadeada)

```c
No *removeValor(No *head, int x) {
    No *ant = NULL, *p = head;
    while (p != NULL && p->valor != x) {
        ant = p;
        p = p->next;
    }
    if (p == NULL) return head;          // não achou
    if (ant == NULL) head = p->next;     // era o primeiro
    else ant->next = p->next;            // pula o nó
    free(p);
    return head;
}
```
