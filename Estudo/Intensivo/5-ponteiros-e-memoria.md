# Lição 5 — Ponteiros, memória e lista

## Ponteiro = variável que guarda um endereço

```c
int a = 10;
int *p = &a;      // p guarda o endereço de a
*p = 20;          // muda a para 20
```

```
 p [ end. de a ] ───►  a [ 20 ]
```

| Escrita | Lê-se |
|---|---|
| `&a` | endereço de a |
| `*p` | o valor que está no endereço guardado em p |
| `int *p;` | (na declaração) p é ponteiro para int |

- Ponteiro sem valor aponta para lixo. Use `int *p = NULL;` até ter um endereço.

## Ponteiro e vetor

```c
int v[3] = {1, 5, 9};
int *p = v;       // o nome do vetor já é o endereço de v[0]
```

- `v[i]` é o mesmo que `*(v + i)`.
- `p + 1` anda **um elemento** (4 bytes para `int`), não 1 byte.
- `*p++` → lê o valor e **anda o ponteiro**. `(*p)++` → **soma 1 no valor**.

## Memória: pilha e heap

| Pilha (stack) | Heap |
|---|---|
| variáveis locais | o que vem do `malloc` |
| some quando a função termina | fica até alguém dar `free` |

## `malloc` e `free`

```c
#include <stdlib.h>

int n = 5;
int *v = malloc(n * sizeof(int));   // vetor de 5 ints no heap
if (v == NULL)                      // malloc devolve NULL se faltar memória
    return 1;
v[0] = 42;                          // usa como vetor normal
free(v);                            // devolve a memória
```

- `malloc` devolve memória **com lixo**. `calloc` devolve zerada.
- Esqueceu o `free` → **vazamento de memória** (memory leak).
- Depois do `free`, não use mais `v`.

## Lista encadeada

Cada nó guarda um valor e o endereço do próximo. O último aponta para `NULL`.

```c
struct no {
    int valor;
    struct no *prox;
};
```

```
inicio ─► [ 3 | ●]─► [ 8 | ●]─► [ 5 | NULL]
```

**Percorrer** (decore este laço):

```c
for (struct no *p = inicio; p != NULL; p = p->prox)
    printf("%d ", p->valor);
```

**Inserir no início:**

```c
struct no *novo = malloc(sizeof(struct no));
novo->valor = 7;
novo->prox = inicio;
inicio = novo;
```

## Teste rápido

1. Com `int a = 3; int *p = &a; *p = *p + 2;`, quanto vale `a`?
2. O que falta neste código? `int *v = malloc(10 * sizeof(int)); v[0] = 1;`
3. Escreva uma função que conta os nós de uma lista.

---

**Gabarito**

1. `5`.
2. O `free(v);` no fim (e, por boa prática, testar se `v == NULL`).
3. ```c
   int contaNos(struct no *inicio) {
       int cont = 0;
       for (struct no *p = inicio; p != NULL; p = p->prox)
           cont++;
       return cont;
   }
   ```
