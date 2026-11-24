# Lição 3 — Funções

## Anatomia

```c
int soma(int a, int b) {    // tipo do retorno, nome, parâmetros
    return a + b;           // devolve o resultado
}

int main() {
    int r = soma(3, 4);     // chamada: r vale 7
    printf("%d\n", r);
    return 0;
}
```

- `void` = não devolve nada (e não tem `return valor`).
- A função vem **antes** do `main`. Se vier depois, coloque o protótipo em cima: `int soma(int a, int b);`
- `return` encerra a função na hora.
- Não dá para criar uma função dentro de outra.

## Passagem por valor (o padrão)

A função recebe uma **cópia**. Mudar a cópia não muda o original.

```c
void dobra(int x) {
    x = x * 2;              // muda só a cópia
}

int a = 5;
dobra(a);
printf("%d\n", a);          // 5  (não mudou!)
```

## Passagem por ponteiro ("por referência")

Para a função mudar a variável de quem chamou: **mande o endereço** (`&a`) e use `*` lá dentro.

```c
void dobra(int *x) {        // x guarda um endereço
    *x = *x * 2;            // *x = "a variável que está lá"
}

int a = 5;
dobra(&a);                  // manda o endereço de a
printf("%d\n", a);          // 10
```

Regra de bolso: **`&` na chamada, `*` na função.**

## O clássico: trocar dois valores

```c
void troca(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}
// troca(&x, &y);
```

## Devolver dois resultados

`return` devolve só um valor. Para mais, use ponteiros:

```c
void divide(int a, int b, int *q, int *r) {
    *q = a / b;
    *r = a % b;
}
// divide(17, 5, &q, &r);  → q = 3, r = 2
```

## Vetor como parâmetro

Vetor **já vai pelo endereço** (sem `&`). A função mexe no original. Mande junto o tamanho.

```c
int somaVetor(int v[], int n) {
    int s = 0;
    for (int i = 0; i < n; i++)
        s = s + v[i];
    return s;
}

int v[4] = {1, 2, 3, 4};
printf("%d\n", somaVetor(v, 4));   // 10
```

## Teste rápido

1. O que imprime?
   ```c
   void zera(int x) { x = 0; }
   // no main:
   int a = 7;
   zera(a);
   printf("%d\n", a);
   ```
2. Corrija a função do item 1 para que ela zere de verdade (e a chamada).
3. Escreva `int maior(int v[], int n)` que devolve o maior elemento.

---

**Gabarito**

1. `7` (passou cópia).
2. `void zera(int *x) { *x = 0; }` e a chamada `zera(&a);`
3. ```c
   int maior(int v[], int n) {
       int m = v[0];
       for (int i = 1; i < n; i++)
           if (v[i] > m)
               m = v[i];
       return m;
   }
   ```
