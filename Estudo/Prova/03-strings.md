# 03 — Strings

## O que é

- String = **vetor de `char` terminado por `'\0'`** (o caractere nulo).
- `char s[] = "Hello";` ocupa **6** bytes: 5 letras + `'\0'`. O compilador coloca o `'\0'` sozinho.
- Mesma coisa, escrito à mão: `char s[] = {'H','e','l','l','o','\0'};`

```c
char s[] = "Hello";
strlen(s);    // 5  (não conta o '\0')
sizeof(s);    // 6  (conta o '\0')
```

- O vetor precisa de espaço para o `'\0'`: `char s[5] = "Hello";` fica **sem** terminador.
- Tudo que lê string para no primeiro `'\0'`. Se `t = "casa"` e fizermos `t[2] = '\0'`, `printf` mostra `ca`.
- `'a'` (aspas simples) é um char. `"a"` (aspas duplas) é uma string de 2 bytes.

## Funções de `<string.h>`

| Função | Faz |
|---|---|
| `strlen(s)` | tamanho, sem o `'\0'` |
| `strcpy(dest, orig)` | copia orig para dest |
| `strcat(dest, orig)` | cola orig no fim de dest |
| `strcmp(a, b)` | 0 se iguais; <0 se a vem antes; >0 se depois |
| `strchr(s, c)` | ponteiro para a 1ª ocorrência do char c |
| `strstr(s, sub)` | ponteiro para a 1ª ocorrência da substring |

- `strcpy`/`strcat` **não conferem tamanho**: o destino tem que caber tudo.
- Não dá para atribuir string com `=` nem comparar com `==`:
  - `s = "oi";` → erro de compilação (vetor não recebe atribuição). Use `strcpy`.
  - `if (a == b)` compara **endereços**, não o texto. Use `strcmp(a, b) == 0`.

## Percorrer uma string

```c
int conta(char s[]) {
    int i = 0;
    while (s[i] != '\0')
        i++;
    return i;              // é o strlen feito à mão
}
```

## Ler string

- `scanf("%s", nome);` → **sem `&`** (o nome do vetor já é endereço). Para no primeiro espaço.
- `fgets(nome, 50, stdin);` → lê a linha inteira (inclui o `'\n'`), no máximo 49 chars.

## Para procurar no YouTube

Módulo no Moodle: **Strings, structs e unions**

- strings em C caractere nulo
- biblioteca string.h strlen strcpy strcat strcmp
- ler string com scanf e fgets em C
