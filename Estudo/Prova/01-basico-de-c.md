# 01 — O básico de C

## Estrutura de um programa

```c
#include <stdio.h>      // diretiva: inclui um cabeçalho

int main() {            // main é obrigatória: o programa começa aqui
    printf("Oi\n");     // comando: termina com ;
    return 0;           // 0 = terminou bem
}
```

- Três recursos básicos: **diretivas** (`#include`, sem `;`), **funções** e **comandos** (com `;`).
- `{ }` fazem o papel de begin/end.
- C é **case-sensitive**: `job`, `Job` e `JOB` são três nomes diferentes.
- Identificador começa com letra ou `_`. `10vezes` e `get-next` são ilegais.

## Do código ao executável (compilação)

`.c` → **pré-processador** (trata `#include`, `#define`) → **compilador** (gera `.o`, código objeto) → **ligador/linker** (junta os `.o` com as bibliotecas) → executável.

```
gcc programa.c -o programa
./programa
```

## Tipos e tamanhos (o mais comum hoje)

| Tipo | Bytes |
|---|---|
| `char` | 1 |
| `short` | 2 |
| `int`, `float` | 4 |
| `double`, `long long` | 8 |
| qualquer ponteiro (64 bits) | 8 |

- `unsigned` = só positivos (dobra o máximo). `signed` é o padrão.
- O tamanho é o **mínimo** garantido; pode mudar com a máquina. Use `sizeof` para saber.
- `sizeof` dá o tamanho em **bytes** e é calculado em tempo de **compilação**.
- `char` é um número de 1 byte: `'A' + 1` vale 66.

## Divisão inteira (pegadinha clássica)

- `7 / 2` → `3` (int com int descarta a parte decimal).
- `7 % 2` → `1` (resto; só para inteiros).
- `7 / 2.0` → `3.5` (basta um operando ser real).
- `5 / 9 * (F - 32)` dá **sempre 0**, porque `5 / 9` é 0. Correção: `5.0 / 9`.

## Entrada e saída (`stdio.h`)

```c
int i;
scanf("%d", &i);          // scanf precisa do ENDEREÇO (&)
printf("i = %d\n", i);    // printf não pula linha sozinho: \n
```

| Formato | Para |
|---|---|
| `%d` | int |
| `%u` | unsigned |
| `%f` | float/double |
| `%c` | um caractere |
| `%s` | string |
| `%x` | hexadecimal |
| `%p` | endereço (ponteiro) |
| `%zu` | `size_t` (resultado de `sizeof`/`strlen`) |

## Variáveis

- Declarar **antes** de usar.
- Variável local **não inicializada** tem lixo → comportamento indefinido.
- `int a, b, c = 10;` → só `c` vale 10.
- `2150.48f` é float; sem o `f` é double.

## Incremento

- `b = a++;` → b recebe o valor **antigo**, depois a aumenta.
- `b = ++a;` → a aumenta **primeiro**, b recebe o novo.

## Controle de fluxo

- Seleção: `if`, `switch`. Repetição: `while`, `do...while`, `for`.
- `do...while` executa **pelo menos uma vez**.
- `break` sai do laço; `continue` pula para a próxima volta.
- `switch` sem `break` **continua** nos `case` de baixo (fall-through).
- `if (x = 5)` **atribui** 5 e é sempre verdadeiro. Comparar é `==`.
- Em C, 0 é falso e qualquer outro valor é verdadeiro.

## Vetores e matrizes

```c
int v[10];        // índices 0 a 9
int m[4][4];      // 16 inteiros
```

- Todos os elementos são do mesmo tipo (estrutura **homogênea**).
- C **não confere** se o índice passou do limite: `v[10]` compila, mas é erro.

## Para procurar no YouTube

Módulo no Moodle: **Primeiros Passos em C + Introdução à Linguagem C**

- introdução linguagem C primeiro programa
- processo de compilação em C pré-processador compilador linker
- tipos de dados em C sizeof
- printf e scanf em C especificadores de formato
- divisão inteira e operador módulo em C
- operadores de incremento i++ e ++i em C
- if else e switch case em C
- while do while e for em C
- vetores e matrizes em C
