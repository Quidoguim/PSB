# Lição 4 — Strings e structs

## String = vetor de `char` que termina em `'\0'`

```c
char nome[10] = "Ana";
```

```
nome:  'A' 'n' 'a' '\0'  ? ? ? ? ? ?
        0   1   2   3
```

- O `'\0'` marca o fim. `"Ana"` ocupa **4** bytes.
- `'A'` (aspas simples) é um caractere. `"A"` (duplas) é uma string.

## Funções de `<string.h>`

| Função | Faz |
|---|---|
| `strlen(s)` | tamanho, **sem** contar o `'\0'` |
| `strcpy(dest, orig)` | copia orig para dest |
| `strcat(dest, orig)` | cola orig no fim de dest |
| `strcmp(a, b)` | **0** se iguais |

- **Não pode** `nome = "Bia";` → use `strcpy(nome, "Bia");`
- **Não compare** com `==` → use `if (strcmp(a, b) == 0)`.
- `scanf("%s", nome);` → **sem `&`** (vetor já é endereço). Para no primeiro espaço.

## Percorrer uma string

Mesmo laço de vetor, mas para no `'\0'`:

```c
int contaA(char s[]) {
    int cont = 0;
    for (int i = 0; s[i] != '\0'; i++)
        if (s[i] == 'a')
            cont++;
    return cont;
}
// contaA("banana") → 3
```

## Struct = vários campos juntos

```c
typedef struct {
    char nome[30];
    float nota;
} Aluno;

Aluno a;
strcpy(a.nome, "Ana");    // campo com ponto
a.nota = 8.5;
```

- `typedef` dá um nome curto (`Aluno`) para o tipo.
- Struct pode ser copiada inteira: `Aluno b = a;`

## Ponteiro para struct: seta `->`

```c
Aluno *p = &a;
p->nota = 9;              // igual a (*p).nota = 9
```

## Vetor de structs

```c
float media(Aluno v[], int n) {
    float s = 0;
    for (int i = 0; i < n; i++)
        s = s + v[i].nota;
    return s / n;
}
```

## Union (só para o V ou F)

Parece struct, mas os campos **dividem a mesma memória**: só um vale por vez. O tamanho é o do **maior** campo.

## Teste rápido

1. Quantos bytes ocupa `"prova"`? E quanto dá `strlen("prova")`?
2. O que há de errado em `if (s == "sim")`?
3. Com `Aluno *p`, escreva duas formas de pôr 10 na nota.

---

**Gabarito**

1. 6 bytes; `strlen` dá 5.
2. `==` compara endereços, não o texto. Certo: `if (strcmp(s, "sim") == 0)`.
3. `p->nota = 10;` e `(*p).nota = 10;`
