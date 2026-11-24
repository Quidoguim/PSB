# Lição 1 — O mínimo de C

## O molde de todo programa

Decore estas 6 linhas. Todo programa em C começa assim.

```c
#include <stdio.h>          // traz printf e scanf

int main() {                // o programa começa aqui
    printf("Ola!\n");       // todo comando termina com ;
    return 0;               // 0 = terminou bem
}
```

- `{ }` abrem e fecham um bloco.
- C diferencia maiúscula de minúscula: `Main` não é `main`.

## Variáveis

Uma variável é uma caixinha com **tipo** e **nome**. Declare antes de usar.

```c
int idade = 20;        // número inteiro
float nota = 7.5;      // número com vírgula (usa ponto!)
char letra = 'A';      // um caractere, aspas simples
```

| Tipo | Guarda | No `printf`/`scanf` |
|---|---|---|
| `int` | inteiro | `%d` |
| `float` | real | `%f` |
| `double` | real (mais preciso) | `%lf` no scanf, `%f` no printf |
| `char` | um caractere | `%c` |

- Variável sem valor inicial tem **lixo** (não é zero).

## Mostrar na tela: `printf`

```c
int a = 3, b = 4;
printf("Soma: %d\n", a + b);         // Soma: 7
printf("%d e %d\n", a, b);           // 3 e 4
printf("Media: %.1f\n", 8.333);      // Media: 8.3   (%.1f = 1 casa decimal)
```

- Cada `%d` é trocado por um valor, na ordem.
- `\n` pula a linha. Sem ele, tudo sai grudado.

## Ler do teclado: `scanf`

```c
int idade;
scanf("%d", &idade);     // o & é obrigatório
```

- `&idade` = "o endereço de idade": o `scanf` precisa saber **onde** guardar.

## Contas

| Operador | Faz | Exemplo |
|---|---|---|
| `+ - *` | soma, subtrai, multiplica | `3 * 4` → 12 |
| `/` | divide | `7 / 2` → **3** (int com int corta a vírgula) |
| `%` | resto (só inteiro) | `7 % 2` → 1 |

- `7 / 2.0` → 3.5. Basta um dos dois ter ponto.
- Número par: `n % 2 == 0`.
- `x++` soma 1 em x. `x--` tira 1.

## Exemplo completo: média de duas notas

```c
#include <stdio.h>

int main() {
    float n1, n2, media;
    printf("Digite duas notas: ");
    scanf("%f %f", &n1, &n2);
    media = (n1 + n2) / 2;
    printf("Media: %.1f\n", media);
    return 0;
}
```

Entrada `6 9` → saída `Media: 7.5`.

## Teste rápido

1. Quanto vale `17 / 5`? E `17 % 5`?
2. O que falta em `scanf("%d", x);`?
3. Escreva um programa que lê um inteiro e mostra o dobro dele.

---

**Gabarito**

1. `3` e `2`.
2. O `&`: `scanf("%d", &x);`
3. ```c
   #include <stdio.h>

   int main() {
       int n;
       scanf("%d", &n);
       printf("%d\n", n * 2);
       return 0;
   }
   ```
