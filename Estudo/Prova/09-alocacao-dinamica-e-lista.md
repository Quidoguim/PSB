# 09 — Alocação dinâmica e lista encadeada

## Memória de um programa

| Região | Guarda |
|---|---|
| Pilha (stack) | variáveis locais e parâmetros; somem quando a função retorna; pequena (~8 MB) |
| Heap | o que veio de `malloc`; fica até alguém dar `free` |
| Dados / BSS | globais e `static` (BSS = as não inicializadas, começam em 0) |
| Texto | o código do programa |

## As funções (`<stdlib.h>`)

| Função | Faz |
|---|---|
| `malloc(n)` | reserva n **bytes**, com lixo dentro |
| `calloc(qtd, tam)` | reserva qtd × tam bytes **zerados** |
| `realloc(p, n)` | muda o tamanho do bloco (pode mudar de lugar) |
| `free(p)` | devolve o bloco |

```c
int *v = malloc(n * sizeof(int));
if (v == NULL) return 1;       // malloc falhou
v[0] = 7;
free(v);
```

- Devolvem `void *` (ou `NULL` se não houver memória).
- Tamanho sempre em bytes → usar `sizeof`.
- C **não tem coletor de lixo**. Alocou e não liberou = **vazamento** (*memory leak*).
- Depois de `free(p)`, p continua com o endereço antigo, mas **não pode** mais ser usado.
- `free` duas vezes no mesmo ponteiro = erro.

## Retornar memória de uma função

```c
int *errado() { int x = 5; return &x; }   // x morre ao retornar!
int *certo()  { int *x = malloc(sizeof(int)); *x = 5; return x; }
```

## Lista simplesmente encadeada

Cada nó tem o valor e o endereço do próximo. O último aponta para `NULL`.

```
head → [3|•] → [8|•] → [1|NULL]
```

```c
typedef struct no {
    int valor;
    struct no *next;
} No;
```

### Inserir no início

```c
No *novo = malloc(sizeof(No));
novo->valor = x;
novo->next = head;
head = novo;
```

### Mostrar

```c
for (No *p = head; p != NULL; p = p->next)
    printf("%d ", p->valor);
```

### Liberar tudo

```c
while (head != NULL) {
    No *aux = head->next;   // guarda o próximo ANTES do free
    free(head);
    head = aux;
}
```

- Se der `free(head)` e depois ler `head->next`, está lendo memória já liberada.

### Lista x vetor

- Lista cresce e diminui à vontade, inserir/remover no início é rápido.
- Vetor acessa `v[i]` direto; na lista é preciso andar nó por nó.

## Para procurar no YouTube

Módulo no Moodle: **Ponteiros e alocação dinâmica de memória (aula Alocação dinâmica)**

- alocação dinâmica em C malloc e free
- malloc calloc realloc diferença
- memória stack e heap
- memory leak vazamento de memória em C
- lista encadeada em C
- inserir e remover em lista encadeada em C
