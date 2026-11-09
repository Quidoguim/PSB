# 06 — Manipulação de bits

## Bases

- A base é só **um jeito de escrever**; o valor guardado é o mesmo.
- Hexa ↔ binário: cada dígito hexa = **4 bits**.
  - `1111 0000 1010 1110` = `F 0 A E` = `0xF0AE`
- Em C: `254`, `0xFE` e `0b11111110` são o mesmo número (`0b` não é padrão, é extensão do GCC).
- `printf("%x", v)` mostra em hexa. **Não existe** formato para binário no `printf`.
- Bit 0 = **LSB** (menos significativo, à direita). Bit 31 = **MSB** num int de 32 bits.

Tabela para decorar:

| Hexa | Binário | | Hexa | Binário |
|---|---|---|---|---|
| 0 | 0000 | | 8 | 1000 |
| 1 | 0001 | | 9 | 1001 |
| 2 | 0010 | | A | 1010 |
| 3 | 0011 | | B | 1011 |
| 4 | 0100 | | C | 1100 |
| 5 | 0101 | | D | 1101 |
| 6 | 0110 | | E | 1110 |
| 7 | 0111 | | F | 1111 |

## Operadores

| Op | Nome | Bit a bit |
|---|---|---|
| `&` | AND | 1 só se os dois forem 1 |
| `\|` | OR | 1 se algum for 1 |
| `^` | XOR | 1 se forem diferentes |
| `~` | NOT | inverte todos |
| `<<` | shift esquerda | empurra para a esquerda, entra 0 |
| `>>` | shift direita | empurra para a direita |

- Usar com inteiros, de preferência `unsigned`.
- `x << 1` **não muda x**. Para mudar: `x = x << 1;`
- `x << n` = x × 2ⁿ. `x >> n` = x ÷ 2ⁿ (inteira).
- `>>` num número **com sinal** e negativo pode entrar 1 à esquerda (depende do compilador). Por isso, `unsigned`.
- Não confundir: `&` e `|` são bit a bit; `&&` e `||` são lógicos (dão 0 ou 1).

Exemplo (x = 0x17 = 0001 0111 = 23):
- `x << 1` = 0010 1110 = 46
- `x >> 1` = 0000 1011 = 11

## As 5 operações do exercício

```c
unsigned int clear(unsigned int x)          { return 0; }
unsigned int setbit(unsigned int x, int b)  { return x | (1u << b); }
unsigned int clearbit(unsigned int x, int b){ return x & ~(1u << b); }
unsigned int invertBit(unsigned int x, int b){ return x ^ (1u << b); }
int testBit(unsigned int x, int b)          { return (x >> b) & 1; }
```

Regra: `1 << b` é a **máscara** com só o bit b ligado.
- ligar → `|`  •  desligar → `& ~`  •  inverter → `^`  •  testar → `>>` e `& 1`

## Campos de bits (cor RGB em um int)

Red bits 0–9, Green 10–19, Blue 20–29 (10 bits cada = 0 a 1023). `0x3FF` = dez bits 1.

```c
// ler o verde
g = (cor >> 10) & 0x3FF;
// gravar 500 no verde: limpa os 10 bits e depois coloca
cor = (cor & ~(0x3FF << 10)) | (500 << 10);
```

Com struct (o compilador faz as contas):

```c
struct rgb { unsigned int red:10, green:10, blue:10; };
```

- Problema da struct com campos de bits: **não se sabe** a ordem dos bits nem o tamanho total (depende do compilador). `__attribute__((packed))` (GCC) evita sobra.

## Para procurar no YouTube

Módulo no Moodle: **Manipulação de bits**

- conversão binário hexadecimal
- operadores bit a bit em C
- operadores de deslocamento shift em C
- máscara de bits em C setar limpar testar bit
- campos de bits bit fields em C
