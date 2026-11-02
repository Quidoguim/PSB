# 02 — Structs e typedef

## O que é

Uma `struct` (registro) agrupa variáveis de **tipos diferentes** sob um nome só. Cada variável dentro dela é um **campo** (ou membro). Um vetor guarda vários itens do mesmo tipo; uma struct guarda itens de tipos diferentes.

## Definindo com typedef

O `typedef` dá um nome curto ao tipo, para não precisar escrever `struct` toda vez:

```c
typedef struct {
    char nome[40];
    float media;
    int faltas;
} Aluno;

Aluno a;          /* variável do tipo Aluno */
a.media = 8.5f;   /* acesso a campo com "." */
```

- A definição vem antes das funções que usam o tipo.
- Uma struct pode ser copiada inteira com `=` (`b = a;` copia todos os campos).

## Struct que aponta para si mesma

Para um nó de lista, o campo `next` aponta para outro nó do **mesmo tipo**. Dentro das chaves o apelido do `typedef` ainda não existe, então a struct precisa de um nome próprio (*tag*):

```c
typedef struct no {
    int valor;
    struct no *next;   /* usa a tag "struct no", não o apelido */
} No;
```

É exatamente o formato do `allocation_t` do T2 (`struct allocation *next`).

## Struct por ponteiro e o operador `->`

Passar struct por valor copia todos os campos; por ponteiro, copia só o endereço (mais barato). Com ponteiro, o acesso a campo é `(*p).campo`, que tem a forma curta `p->campo`:

```c
void falta(Aluno *p) { p->faltas++; }   /* igual a (*p).faltas++ */
```

## No T2

- `allocation_t` = nó da lista (`start`, `size`, `next`).
- `mymemory_t` = gerenciador (`pool`, `total_size`, `head`).
- Quase tudo no código vai ser `memory->head`, `atual->start`, `atual->next`.

---
Fontes: Moodle › Strings, structs e unions — slides "Strings, structs e unions" (Prof. Eduarda), "Structs (Registros)" (Prof. Marcelo Cohen, inf.pucrs.br/flash/progswb2/present/U01_progC/struct), exemplo "Frações".
