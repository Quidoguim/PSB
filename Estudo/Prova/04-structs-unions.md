# 04 — Structs, typedef e unions

## Struct

- Agrupa campos de **tipos diferentes** sob um nome (estrutura **heterogênea**). Vetor é homogêneo.
- Cada item é um **campo** ou **membro**.

```c
struct aluno {
    float p1, p2;
    int faltas;
};

struct aluno a;
a.p1 = 7.5;          // acesso com ponto
```

## typedef

Dá um apelido ao tipo para não repetir `struct`:

```c
typedef struct {
    int num, den;
} Fracao;

Fracao f = {1, 2};
```

## Atribuição

- Struct pode ser copiada inteira com `=`: `b = a;` copia todos os campos.
- Depois da cópia, mudar `b` não muda `a`.
- Struct **não** pode ser comparada com `==`. Compare campo a campo.

## Struct em função

- **Por valor**: copia a struct inteira (mais lento). Mudanças não voltam.
- **Por referência**: passa só o endereço (mais barato, **recomendado**). Acesso com `->`.

```c
void zeraX(struct ponto *p) {
    p->x = 0;        // igual a (*p).x = 0
}
// chamada: zeraX(&pt);
```

- `p->x` é atalho para `(*p).x`. Os parênteses importam: `*p.x` está errado.

## Vetor de structs

```c
struct aluno turma[30];
turma[0].p1 = 8.0;
```

## Union

- Parece struct, mas **todos os campos dividem a mesma memória**.
- Só **um** campo vale por vez: escrever num estraga os outros.
- Tamanho = o do **maior** campo.

```c
union dado { int i; float f; char s[20]; };
// sizeof(union dado) é 20 (o maior campo)
```

| | struct | union |
|---|---|---|
| Memória | um espaço para cada campo | um espaço só, compartilhado |
| Tamanho | soma dos campos (+ alinhamento) | o maior campo |
| Campos válidos | todos | um por vez |

## Para procurar no YouTube

Módulo no Moodle: **Strings, structs e unions**

- struct em C
- typedef struct em C
- ponteiro para struct operador seta em C
- vetor de struct em C
- union em C diferença entre struct e union
