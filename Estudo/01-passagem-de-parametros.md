# 01 — Funções e passagem de parâmetros

## Declaração x definição

- **Protótipo (declaração):** tipo de retorno + nome + parâmetros, terminado em `;`. Avisa o compilador que a função existe.
- **Definição:** o protótipo com o corpo `{ ... }`.
- Se a função é usada antes de ser definida (por exemplo, definida depois do `main`), o protótipo é obrigatório. Em projetos modulares, os protótipos ficam no `.h` (ver [05](05-modularizacao-e-makefile.md)).
- Uma função não pode ser definida dentro de outra.

## Parâmetros

Os parâmetros se comportam como variáveis locais: nascem quando a função é chamada e somem quando ela retorna.

### Por valor

A função recebe uma **cópia**. Mudar o parâmetro não muda a variável de quem chamou.

```c
int dobro(int x) { x = x * 2; return x; }
/* int t = 5; dobro(t);  -> t continua 5 */
```

### Por referência (com ponteiro)

C só passa por valor; para "passar por referência" passamos o **endereço** da variável (`&`) e a função acessa o conteúdo com `*`.

```c
void zera(int *x) { *x = 0; }
/* int t = 5; zera(&t);  -> t vira 0 */
```

Usos típicos:
- a função precisa **alterar** a variável de quem chamou;
- a função precisa "devolver" **mais de um valor** (ex.: `swap(&a, &b)`);
- evitar copiar uma struct grande (passa só o ponteiro — ver [02](02-structs.md)).

### Vetores

Vetores são sempre passados como ponteiro para o primeiro elemento, então a função mexe no vetor original. `int v[]` e `int *v` no parâmetro significam a mesma coisa. O tamanho tem que ir num parâmetro separado.

## No T2

Todas as funções da biblioteca recebem `mymemory_t *memory`: um ponteiro, para que `mymemory_alloc` e `mymemory_free` alterem a lista **original** (o `head` dentro da struct), e não uma cópia.

---
Fontes: Moodle › Passagem de parâmetros — slides "Passagem de parâmetros em C" (Prof. Eduarda) e "Funções em C — Passagem de Parâmetros" (Prof. Marcelo Cohen, inf.pucrs.br/flash/progswb2/aulas/funcoes).
