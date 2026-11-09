# 08 — Ponteiros

## Os dois operadores

- `&x` → **endereço** de x.
- `*p` → **conteúdo** do endereço guardado em p (desreferência).
- Na declaração, `*` só diz "é ponteiro": `int *p;`

```c
int b = 200;
int *a = &b;     // a guarda o endereço de b
*a = 5;          // muda b para 5
printf("%p", a); // mostra o endereço (hexa)
```

Desenho para o caderno:

```
 a [ 0x100 ] ───►  b [ 200 ]   (b mora no endereço 0x100)
```

## Ponteiro inválido e NULL

- Ponteiro não inicializado aponta para **lixo**. Usar `*p` → falha de segmentação (*segfault*).
- Boa prática: `int *p = NULL;` e testar `if (p != NULL)` (ou `if (p)`).
- `NULL` vale **zero**.

## Ponteiros e vetores

- O nome do vetor já é o endereço do 1º elemento: `int *p = v;` (sem `&`).
- `v[i]` é o mesmo que `*(v + i)`. E `p[i]` também funciona.
- Diferença: `p` pode mudar (`p++`), o nome do vetor `v` não.

## Aritmética de ponteiros

`p + 1` anda **um elemento**, não um byte: soma `sizeof(tipo)` bytes.

| Ponteiro | `p + 1` anda |
|---|---|
| `char *` | 1 byte |
| `int *` | 4 bytes |
| `double *` | 8 bytes |

- `q - p` (mesmo vetor) = **quantos elementos** entre eles.
- `p < q` = quem vem antes na memória.
- Somar dois ponteiros (`p + q`) **não** é permitido.

## A pegadinha de prova

```c
int v[3] = {1, 5, 9};
int *p = v;
```

| Expressão | Faz |
|---|---|
| `*p++` | lê `*p` e depois **avança o ponteiro** |
| `(*p)++` | aumenta o **valor apontado**; p fica parado |
| `*(p + 1)` | lê o próximo, sem mudar p |

Num laço de 3 voltas: `*ptr++` mostra `1 5 9`; `(*ptr)++` mostra `1 2 3` (e v[0] termina 4).

## Percorrer vetor sem `[]`

```c
int busca(int *v, int n, int x) {
    for (int *p = v; p < v + n; p++)
        if (*p == x) return p - v;   // índice
    return -1;
}
```

## `char *` e `void *`

- `char *` anda de **1 em 1 byte**: serve para ver os bytes de qualquer coisa.
- `void *` = ponteiro "sem tipo" (o que `malloc` devolve). Para usar, converte: `(char *)p + n`.

## Para procurar no YouTube

Módulo no Moodle: **Ponteiros e alocação dinâmica de memória (aula Ponteiros)**

- ponteiros em C para iniciantes
- operadores & e * em C
- ponteiro NULL em C
- ponteiros e vetores em C
- aritmética de ponteiros em C
