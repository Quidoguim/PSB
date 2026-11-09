# 11 — Treino: o que o código imprime?

Resolva no caderno **antes** de olhar o gabarito. Todas as respostas foram conferidas compilando e rodando (macOS, int de 4 bytes, ponteiro de 8). Considere os `#include` necessários.

## A. Operadores

```c
printf("%d %d\n", 7 / 2, 7 % 2);                 // A1
float c = 5 / 9 * (100 - 32); printf("%f\n", c); // A2
int a = 5, b; b = a++; printf("%d %d\n", a, b);  // A3
a = 5; b = ++a; printf("%d %d\n", a, b);         // A4
printf("%d %c\n", 'A' + 1, 'A' + 2);             // A5
```

## B. Controle de fluxo

```c
// B1
int k = 0;
for (int i = 0; i < 5; i++) { if (i == 3) break; k += i; }
printf("%d\n", k);

// B2
k = 0;
for (int i = 0; i < 5; i++) { if (i % 2 == 0) continue; k += i; }
printf("%d\n", k);

// B3
int sw = 2;
switch (sw) {
    case 1: printf("um ");
    case 2: printf("dois ");
    case 3: printf("tres "); break;
    default: printf("outro");
}

// B4
int i = 10;
do { i++; } while (i < 5);
printf("%d\n", i);
```

## C. Funções e parâmetros

```c
void dobra(int x)     { x = x * 2; }
void dobraR(int *x)   { *x = *x * 2; }
void zera(int v[], int n) { for (int i = 0; i < n; i++) v[i] = 0; }

int x = 10;
dobra(x);   printf("%d\n", x);          // C1
dobraR(&x); printf("%d\n", x);          // C2
int v[5] = {1, 2, 3, 4, 5};
zera(v, 3);
printf("%d %d %d\n", v[0], v[3], v[4]); // C3
```

## D. Ponteiros

```c
int w[3] = {10, 20, 30};
int *p = w;
printf("%d %d %d\n", *p, *(p + 2), p[1]);   // D1

p = w;
printf("%d ", *p++);  printf("%d\n", *p);   // D2

p = w;
printf("%d ", (*p)++); printf("%d\n", w[0]); // D3

int *q = &w[2], *r = &w[0];
printf("%ld\n", (long)(q - r));             // D4

int vv[4] = {4, 3, 2, 1};
int *e = vv + 3;
printf("%d %d\n", *e, *(e - 2));            // D5
```

## E. Strings

```c
char s[] = "Hello";
printf("%zu %zu\n", strlen(s), sizeof(s));        // E1

char s2[10] = "ab"; strcat(s2, "cd");
printf("%s %zu\n", s2, strlen(s2));               // E2

char t[] = "casa"; t[2] = '\0';
printf("%s %zu %zu\n", t, strlen(t), sizeof(t));  // E3

char n[] = "Ana"; n[0] = 'E'; printf("%s\n", n);  // E4
```

## F. Structs e union

```c
struct ponto { int x, y; };
void moveV(struct ponto p)  { p.x = 100; }
void moveR(struct ponto *p) { p->x = 100; }

struct ponto pt = {1, 2};
moveV(pt);  printf("%d ", pt.x);
moveR(&pt); printf("%d\n", pt.x);         // F1

struct ponto pa = {3, 4}, pb;
pb = pa; pb.x = 9;
printf("%d %d\n", pa.x, pb.x);            // F2

union dado { int i; char c[8]; double d; };
printf("%zu\n", sizeof(union dado));      // F3
```

## G. Bits

```c
unsigned char u = 0x17;                          // 0001 0111
printf("%d %d %d\n", u << 1, u >> 1, u >> 3);    // G1

unsigned int m = 0x4A, n = 0x92;
printf("%02x %02x %02x\n", m & n, m | n, m ^ n); // G2

unsigned int f = 0;  f = f | (1u << 3);  printf("%u\n", f);       // G3
f = 0xFF;  f = f & ~(1u << 0);  printf("%u %x\n", f, f);          // G4
printf("%d\n", (0x2C >> 2) & 1);                                  // G5
```

## H. Achar o erro

Diga o que está errado em cada um (não precisa rodar).

```c
int *p; *p = 10;                          // H1
char s[5] = "Hello";                      // H2
char a[10]; a = "oi";                     // H3
int *f() { int x = 3; return &x; }        // H4
free(p); printf("%d", *p);                // H5
if (strA == strB) printf("iguais");       // H6
FILE *fp = fopen("x.txt", "r"); fprintf(fp, "oi"); // H7 (dois problemas)
```

---

## Gabarito

| Q | Saída | Por quê |
|---|---|---|
| A1 | `3 1` | divisão inteira e resto |
| A2 | `0.000000` | `5 / 9` é 0 (int/int) |
| A3 | `6 5` | `a++` entrega o valor antigo |
| A4 | `6 6` | `++a` incrementa antes |
| A5 | `66 C` | char é número; 'A' = 65 |
| B1 | `3` | 0+1+2, para no i == 3 |
| B2 | `4` | só ímpares: 1+3 |
| B3 | `dois tres ` | sem `break` no case 2, cai no 3 |
| B4 | `11` | do-while roda uma vez antes de testar |
| C1 | `10` | por valor: mudou só a cópia |
| C2 | `20` | por referência |
| C3 | `0 4 5` | vetor vai por referência; zerou os 3 primeiros |
| D1 | `10 30 20` | `*(p+2)` = w[2], `p[1]` = w[1] |
| D2 | `10 20` | lê 10, depois p anda |
| D3 | `10 11` | lê 10, depois o **valor** vira 11 |
| D4 | `2` | diferença em elementos, não bytes |
| D5 | `1 3` | e aponta vv[3]; e - 2 é vv[1] |
| E1 | `5 6` | sizeof conta o `'\0'` |
| E2 | `abcd 4` | strcat cola no fim |
| E3 | `ca 2 5` | a string termina no novo `'\0'`; o vetor continua com 5 bytes |
| E4 | `Ena` | vetor de char pode ser alterado |
| F1 | `1 100` | por valor não muda; com `->` muda |
| F2 | `3 9` | `=` copia; depois são independentes |
| F3 | `8` | tamanho do maior campo |
| G1 | `46 11 2` | 0010 1110, 0000 1011, 0000 0010 |
| G2 | `02 da d8` | 0100 1010 com 1001 0010 |
| G3 | `8` | liga o bit 3 |
| G4 | `254 fe` | desliga o bit 0 de 1111 1111 |
| G5 | `1` | 0x2C = 101100; bit 2 é 1 |
| H1 | | p não foi inicializado (aponta para lixo) → segfault |
| H2 | | não sobra lugar para o `'\0'`; precisa `[6]` |
| H3 | | vetor não recebe `=`; usar `strcpy(a, "oi")` |
| H4 | | x é local e morre no retorno; usar `malloc` |
| H5 | | uso depois do `free` |
| H6 | | compara endereços; usar `strcmp(strA, strB) == 0` |
| H7 | | não testou `NULL`; e abriu com `"r"` (só leitura) para escrever |
