# Lição 7 — Teste de mesa: descobrir o que imprime

Não tente adivinhar de cabeça. Faça uma tabela: **uma coluna por variável**, **uma linha por passo**. Risque o valor velho e escreva o novo.

## Exemplo

```c
int s = 0;
for (int i = 1; i <= 4; i++)
    s = s + i;
printf("%d\n", s);
```

| passo | i | i <= 4? | s |
|---|---|---|---|
| início | 1 | sim | 0 → 1 |
| | 2 | sim | 1 → 3 |
| | 3 | sim | 3 → 6 |
| | 4 | sim | 6 → 10 |
| | 5 | **não** → sai | 10 |

Imprime `10`.

## Checklist antes de responder

- Divisão é de inteiros? Corte a vírgula.
- O laço usa `<` ou `<=`? Conte a última volta com cuidado.
- O `printf` tem `\n`? Se não tem, as saídas ficam na mesma linha.
- `switch` sem `break`? Continue descendo.
- A função recebeu cópia ou ponteiro? Cópia não muda o original.
- `a++` ou `++a`?

## Treino: o que imprime?

```c
// 1
int a = 5, b = 2;
printf("%d %d\n", a / b, a % b);
```

```c
// 2
int x = 10;
if (x > 5)
    printf("A");
else
    printf("B");
printf("C\n");
```

```c
// 3
int i = 0;
while (i < 10)
    i = i + 3;
printf("%d\n", i);
```

```c
// 4
int a = 5;
int b = a++;
printf("%d %d\n", a, b);
```

```c
// 5
int op = 2;
switch (op) {
    case 1: printf("um ");
    case 2: printf("dois ");
    case 3: printf("tres ");
            break;
    default: printf("outro");
}
```

```c
// 6
void muda(int x) { x = 99; }
// no main:
int a = 1;
muda(a);
printf("%d\n", a);
```

```c
// 7
void muda(int *x) { *x = 99; }
// no main:
int a = 1;
muda(&a);
printf("%d\n", a);
```

```c
// 8
int v[3] = {1, 5, 9};
int *p = v;
p++;
printf("%d\n", *p);
```

```c
// 9
char s[] = "casa";
printf("%zu %zu\n", strlen(s), sizeof(s));   // %zu: formato de strlen e sizeof
```

```c
// 10
int v[4] = {2, 4, 6, 8};
for (int i = 3; i >= 0; i--)
    printf("%d ", v[i]);
```

---

**Gabarito**

| # | Imprime | Por quê |
|---|---|---|
| 1 | `2 1` | 5/2 corta para 2; resto 1. |
| 2 | `AC` | Entra no `if`; o `printf("C")` está fora do if/else. |
| 3 | `12` | i: 0 → 3 → 6 → 9 → 12 (12 < 10 é falso, sai). |
| 4 | `6 5` | b pega o valor antigo (5), depois a vira 6. |
| 5 | `dois tres ` | Entra no case 2 e, sem `break`, desce até o `break` do 3. |
| 6 | `1` | Passou cópia. |
| 7 | `99` | Passou o endereço. |
| 8 | `5` | p andou um elemento: de v[0] para v[1]. |
| 9 | `4 5` | strlen sem `'\0'`; sizeof com. |
| 10 | `8 6 4 2 ` | Laço de trás para frente. |
