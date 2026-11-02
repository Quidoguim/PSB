# 06 — Lista simplesmente encadeada

Referência prática: nosso exercício entregue, [ListaEncadeada/lista.c](../ListaEncadeada/lista.c), feito sobre o código-base da professora.

## A estrutura

Cada **nó** guarda um dado e um ponteiro para o próximo nó. A lista guarda onde começa (`head`). O último nó tem `next == NULL`.

```
head -> [ 7 | * ] -> [ 3 | * ] -> [ 9 | NULL ]
```

Diferente de um vetor, os nós não ficam lado a lado na memória: cada um é um `malloc` separado, ligado pelo `next`.

## Operações (como no exercício)

| Operação | Ideia |
|---|---|
| **Percorrer** (`show`) | `atual = head; while (atual != NULL) { ...; atual = atual->next; }` |
| **Inserir no início** (`addStart`) | novo nó; `novo->next = head`; `head = novo` |
| **Inserir no fim** (`add`) | novo nó com `next = NULL`; o antigo último (`tail`) passa a apontar para ele |
| **Remover por valor** (`removeByValue`) | andar guardando o **anterior**; ao achar, o anterior pula o nó (`anterior->next = atual->next`) ou, se for o primeiro, `head = atual->next`; depois `free(atual)` |
| **Liberar tudo** (`freeList`) | guardar o próximo **antes** do `free` do atual |

```c
No *atual = lista->head;
while (atual != NULL) {
    No *proximo = atual->next;   /* guarda antes: depois do free não pode ler atual->next */
    free(atual);
    atual = proximo;
}
lista->head = NULL;
```

## Caso novo no T2: inserir no meio, mantendo a ordem

No T2 a lista fica **ordenada por endereço**. Ao alocar, o nó novo entra **entre** dois nós existentes:

```c
novo->next = atual->next;   /* 1º: o novo aponta para quem vinha depois */
atual->next = novo;         /* 2º: o anterior passa a apontar para o novo */
```

A ordem das duas linhas importa: se trocar, perdemos o resto da lista.

Os três casos de inserção (início, meio, fim) do T2 são os mesmos do exercício, só que a posição é escolhida pelo endereço.

## No T2

- `allocation_t` é o nó; `mymemory_t` guarda o `head`.
- `mymemory_display` = percorrer.
- `mymemory_free` = remover, comparando `atual->start == ptr` em vez de um inteiro.
- `mymemory_cleanup` = liberar tudo.
- O dado de cada nó não é um `int`: é a descrição de um pedaço do pool (`start`, `size`).

---
Fontes: Moodle › Ponteiros e alocação dinâmica — "Exemplo de início de implementação de lista simplesmente encadeada", "Exercícios de fixação: Alocação Dinâmica" e "ENVIO TAREFA LISTA ENCADEADA".
