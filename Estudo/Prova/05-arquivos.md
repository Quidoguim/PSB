# 05 — Arquivos

## Por que usar

Quando o programa termina, as variáveis somem. Arquivo **guarda os dados** depois do fim.

## Texto x binário

| Texto (.txt, .csv) | Binário (.bin, .dat) |
|---|---|
| dá para ler num editor | não dá para ler direto |
| ocupa mais espaço | mais compacto |
| `fprintf`, `fscanf`, `fgets`, `fgetc`, `fputc` | `fwrite`, `fread` |

## O ponteiro FILE

```c
FILE *fp = fopen("dados.txt", "w");
if (fp == NULL) {               // sempre testar!
    printf("Erro\n");
    return 1;
}
fprintf(fp, "%d\n", 42);
fclose(fp);                     // sempre fechar
```

## Modos do `fopen`

| Modo | Faz |
|---|---|
| `"r"` | só leitura (arquivo tem que existir) |
| `"w"` | só escrita, **apaga** o conteúdo / cria |
| `"a"` | acrescenta **no fim** / cria |
| `"r+"` | leitura e escrita (não apaga) |
| `"w+"` | leitura e escrita (apaga) |
| `"a+"` | leitura e acréscimo |

Acrescentar `b` = binário: `"rb"`, `"wb"`, `"ab"`.

- `fopen` devolve `NULL` se falhar (ex.: `"r"` num arquivo que não existe).
- A escrita é **bufferizada**: só vai mesmo pro disco no `fclose` ou com `fflush(fp)`.

## Binário: fwrite e fread

Os dois têm 4 argumentos: **endereço, tamanho de um item, quantidade, arquivo**.

```c
struct reg r;
fwrite(&r, sizeof(struct reg), 1, fp);   // grava 1 struct
fread(&r, sizeof(struct reg), 1, fp);    // lê 1 struct
```

- Os dois devolvem **quantos itens** conseguiram ler/gravar. No fim do arquivo, `fread` devolve 0.

```c
int n = 0;
while (n < len && fread(&v[n], sizeof(aluno_t), 1, fp) == 1)
    n++;                                  // n = registros lidos
```

## Acesso direto: fseek

`fseek(fp, deslocamento_em_bytes, origem);`

| Origem | A partir de |
|---|---|
| `SEEK_SET` | início do arquivo |
| `SEEK_CUR` | posição atual |
| `SEEK_END` | fim do arquivo |

```c
fseek(fp, 2 * sizeof(struct reg), SEEK_SET);   // pula para o 3º registro
fseek(fp, -sizeof(struct reg), SEEK_END);      // vai para o último
```

## Ler texto

```c
fscanf(fp, "%d", &x);           // igual scanf, mas do arquivo
fgets(linha, 100, fp);          // lê uma linha; NULL no fim
```

## Para procurar no YouTube

Módulo no Moodle: **Arquivos**

- arquivos em C fopen fclose
- modos de abertura de arquivo em C r w a
- fprintf e fscanf em C
- arquivo binário em C fwrite e fread
- fseek em C acesso aleatório
