# 03 — Ponteiros e aritmética de ponteiros

## Conceitos

- Toda variável mora num **endereço** de memória. `&x` dá o endereço de `x`.
- **Ponteiro** é uma variável que guarda um endereço. `int *p = &x;` → `p` aponta para `x`.
- `*p` (desreferência) acessa o **conteúdo** do endereço: `*p = 10;` muda `x`.
- `printf("%p", p)` imprime um endereço em hexadecimal.

## Ponteiro inválido e NULL

- Usar um ponteiro que não foi inicializado é erro grave (normalmente *segmentation fault*).
- Boa prática: inicializar com `NULL` e testar antes de usar: `if (p != NULL) ...`.
- `NULL` é o endereço zero, usado como "não aponta para nada". É também o valor que o `malloc` devolve quando falha e o que marca o fim de uma lista encadeada.

## Ponteiros e vetores

- O nome de um vetor funciona como ponteiro para o primeiro elemento: `int *p = v;` (sem `&`).
- `p[i]` é o mesmo que `*(p + i)`.

## Aritmética de ponteiros

`p + 1` não soma 1 byte: soma **o tamanho do tipo apontado**.

| Tipo do ponteiro | `p + 1` anda |
|---|---|
| `char *` | 1 byte |
| `int *` | 4 bytes (normalmente) |
| `double *` | 8 bytes |

Operações válidas:
- `p + n`, `p - n`, `p++`, `p--` → anda `n` elementos;
- `q - p` (dois ponteiros do mesmo vetor) → **quantos elementos** há entre eles;
- comparar `p < q` → quem vem antes na memória.

Pegadinha clássica: `*p++` lê o valor e **avança o ponteiro**; `(*p)++` incrementa o **valor apontado** e o ponteiro fica parado.

### `char *` como "ponteiro de byte"

Como `char` tem 1 byte, um `char *` anda byte a byte. Um dos exercícios da aula era mostrar cada byte de um `int` ou de uma matriz usando só `char *`. É essa a ferramenta para fazer contas de endereço em bytes.

### `void *`

`void *` é um ponteiro "sem tipo": guarda qualquer endereço, mas não dá para desreferenciar nem somar nele em C padrão (o GCC/Clang aceita a soma como extensão, e avisa com `-pedantic`). Para fazer conta, converte-se antes: `(char *)p + n`.

## No T2

- `pool` e `start` são `void *`. Para achar "o fim de uma alocação": `(char *)atual->start + atual->size`.
- Tamanho de um buraco livre: `(char *)proximo->start - ((char *)atual->start + atual->size)` → subtração de ponteiros `char *` = distância em bytes.
- Posição de uma alocação dentro do pool: `(char *)start - (char *)pool`.

---
Fontes: Moodle › Ponteiros e alocação dinâmica — slides "Ponteiros" (Prof. Eduarda), "Introdução a Ponteiros" (Prof. Marcelo Cohen, inf.pucrs.br/flash/progswb2/present/U02_pointers/pointers), "Exercícios de fixação: Ponteiros" (vetores sem usar `[]`).
