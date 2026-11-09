# 02 — Funções e passagem de parâmetros

## Declaração x definição

- **Protótipo** (declaração): tipo de retorno + nome + parâmetros, com `;`.
- **Definição**: o corpo inteiro da função.
- Se a função aparece **depois** de quem a chama, o protótipo é obrigatório antes.
- Uma função **não** pode ser definida dentro de outra.

```c
int soma(int a, int b);            // protótipo

int main() { return soma(2, 3); }

int soma(int a, int b) {           // definição
    return a + b;
}
```

- Parâmetros são variáveis **locais**: nascem na chamada e somem no retorno.
- `void` = não retorna nada.

## Por valor

A função recebe uma **cópia**. Mudar a cópia não muda o original.

```c
void dobra(int x) { x = x * 2; }
int t = 10;
dobra(t);          // t continua 10
```

## Por referência (com ponteiro)

Passa o **endereço**; a função mexe no original com `*`.

```c
void dobra(int *x) { *x = *x * 2; }
int t = 10;
dobra(&t);         // t vira 20
```

- Quem chama manda o endereço: `&`.
- Quem recebe declara ponteiro e acessa com `*`.
- **Detalhe de V ou F:** C só tem passagem **por valor**. A "por referência" é simulada passando o endereço (por valor).

## Para que serve a referência

1. Alterar a variável de quem chamou (ex.: `swap`).
2. "Retornar" mais de um valor (ex.: maior e menor).

```c
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}
// chamada: swap(&x, &y);
```

## Vetores em funções

- Vetor é **sempre** passado por referência (vai o endereço do 1º elemento).
- Por isso a função **altera** o vetor original.
- `int v[]` e `int *v` no parâmetro são a mesma coisa.
- A função não sabe o tamanho: passe `n` junto.
- Dentro da função, `sizeof(v)` dá o tamanho do **ponteiro** (8), não do vetor.

```c
int maior(int *v, int n) {
    int m = v[0];
    for (int i = 1; i < n; i++)
        if (v[i] > m) m = v[i];
    return m;
}
```

## Comparando com Java

Java também só passa por valor e não tem ponteiros explícitos.

## Para procurar no YouTube

Módulo no Moodle: **Passagem de parâmetros**

- funções em C protótipo e definição
- passagem por valor e por referência em C
- função swap com ponteiros em C
- passando vetor para função em C
