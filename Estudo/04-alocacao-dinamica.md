# 04 — Alocação dinâmica de memória

## Por que existe

Variáveis locais ficam na **pilha** (*stack*), que é pequena: costuma ter 8 MB (`ulimit -s` mostra). Um vetor local de 2 milhões de `double` (16 MB) estoura a pilha e o programa cai.

A alocação dinâmica pega memória do **heap**, que:
- comporta blocos grandes;
- **continua existindo depois que a função retorna** (até alguém dar `free`) — por isso serve para estruturas encadeadas, como listas.

## Mapa da memória de um processo

| Região | Guarda |
|---|---|
| Stack (pilha) | variáveis locais, parâmetros, registradores salvos, endereço de retorno |
| Heap | memória de `malloc`/`calloc`/`realloc` |
| BSS | globais e estáticas **não inicializadas** |
| Data | globais **inicializadas** e constantes (ex.: strings literais) |
| Text | código do programa (instruções) |

A pilha cresce numa direção e o heap na outra; o limite da pilha é fixo, o heap pode crescer bastante.

## As funções (`<stdlib.h>`)

| Função | Faz |
|---|---|
| `void *malloc(size_t n)` | reserva `n` bytes (conteúdo lixo); `NULL` se falhar |
| `void *calloc(size_t qtd, size_t tam)` | reserva `qtd * tam` bytes **zerados** |
| `void *realloc(void *p, size_t n)` | muda o tamanho de um bloco (pode movê-lo de lugar) |
| `void free(void *p)` | devolve o bloco |

- O tamanho é sempre em **bytes**, então usa-se `sizeof`: `int *v = malloc(10 * sizeof(int));`.
- `size_t` é um inteiro **sem sinal**, o tipo certo para tamanhos (imprime com `%zu`).
- **Sempre testar se o retorno é `NULL`.**

## Liberação

- C **não tem coletor de lixo**: tudo que foi alocado tem que ser liberado com `free`. O que não é liberado é um **vazamento de memória** (*memory leak*).
- Liberar na **ordem inversa** da construção quando uma coisa aponta para outra. Exemplo da matriz alocada linha a linha: primeiro `free` de cada linha, depois `free` do vetor de linhas. Se liberar o vetor primeiro, perdemos os ponteiros das linhas.
- Depois do `free`, o ponteiro continua com o endereço antigo, mas a memória não é mais nossa: não usar.

## Conferindo vazamentos no macOS

```bash
leaks --atExit -- ./programa
```

No fim, procurar a linha `0 leaks for 0 total leaked bytes`.

## No T2

O T2 é **escrever uma versão pequena do próprio `malloc`/`free`**:
- `malloc` de verdade só em três lugares: o `mymemory_t`, o pool, e cada nó `allocation_t`;
- o que o usuário recebe de `mymemory_alloc` é um pedaço **de dentro do pool**, nunca um `malloc` novo;
- `mymemory_cleanup` precisa liberar tudo, nesta ordem: os nós da lista, o pool, e por último o `mymemory_t`.

---
Fontes: Moodle › Ponteiros e alocação dinâmica — slides "Alocação dinâmica de memória — aula 10" (PDF postado pela Prof. Eduarda, autoria do Prof. Marcelo Veiga Neves), "Alocação Dinâmica de Memória" (Prof. Marcelo Cohen, inf.pucrs.br/flash/progswb2/present/U02_pointers/dynamic), "Exercícios de fixação: Alocação Dinâmica".
