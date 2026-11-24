# Lição 2 — `if`, laços e vetores

## Decidir: `if` / `else`

```c
if (nota >= 6) {
    printf("Aprovado\n");
} else {
    printf("Reprovado\n");
}
```

| Comparar | | Juntar condições | |
|---|---|---|---|
| `==` | igual | `&&` | E (as duas verdadeiras) |
| `!=` | diferente | `\|\|` | OU (pelo menos uma) |
| `< > <= >=` | menor, maior... | `!` | NÃO |

- **Cuidado:** `if (x = 5)` não compara, **guarda** 5 em x (e é sempre verdadeiro). Comparar é `==`.
- Em C, `0` é falso e qualquer outro número é verdadeiro.

## Repetir: `for` e `while`

```c
for (int i = 0; i < 5; i++) {    // i vale 0, 1, 2, 3, 4 → 5 voltas
    printf("%d ", i);
}
```

Lê-se: "começa em 0; enquanto `i < 5`; no fim de cada volta, `i++`".

```c
int i = 0;
while (i < 5) {       // mesmo laço, escrito com while
    printf("%d ", i);
    i++;
}
```

- `while` testa antes: pode rodar zero vezes.
- `do { ... } while (cond);` testa no fim: roda **pelo menos uma vez**.

## Vetores

```c
int v[5] = {4, 7, 1, 9, 3};   // 5 inteiros
v[0];                          // primeiro: 4
v[4];                          // último: 3  (v[5] NÃO existe)
```

- Índice começa em **0** e vai até **tamanho − 1**.
- C não avisa se você passar do fim. É erro do programador.

## Os 4 padrões que resolvem quase tudo

Decore. A maioria das questões de código é um destes.

```c
// 1. SOMAR
int soma = 0;
for (int i = 0; i < n; i++)
    soma = soma + v[i];

// 2. CONTAR (quantos são pares)
int cont = 0;
for (int i = 0; i < n; i++)
    if (v[i] % 2 == 0)
        cont++;

// 3. MAIOR
int maior = v[0];
for (int i = 1; i < n; i++)
    if (v[i] > maior)
        maior = v[i];

// 4. PROCURAR (posição de x, ou -1)
int pos = -1;
for (int i = 0; i < n; i++)
    if (v[i] == x)
        pos = i;
```

- Soma e contador **começam em 0**. Maior começa no **primeiro elemento**.

## Exemplo completo: ler 5 números, mostrar soma e maior

```c
#include <stdio.h>

int main() {
    int v[5], soma = 0;
    for (int i = 0; i < 5; i++)
        scanf("%d", &v[i]);

    int maior = v[0];
    for (int i = 0; i < 5; i++) {
        soma = soma + v[i];
        if (v[i] > maior)
            maior = v[i];
    }
    printf("Soma: %d  Maior: %d\n", soma, maior);
    return 0;
}
```

Entrada `4 7 1 9 3` → `Soma: 24  Maior: 9`.

## Teste rápido

1. Quantas vezes roda `for (i = 1; i <= 10; i++)`?
2. O que imprime?
   ```c
   for (int i = 0; i < 3; i++)
       printf("%d", i * 2);
   ```
3. Escreva o laço que conta quantos elementos de `v` (tamanho `n`) são maiores que 10.

---

**Gabarito**

1. 10 vezes (de 1 a 10).
2. `024`
3. ```c
   int cont = 0;
   for (int i = 0; i < n; i++)
       if (v[i] > 10)
           cont++;
   ```
