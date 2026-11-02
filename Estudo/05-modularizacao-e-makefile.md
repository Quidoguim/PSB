# 05 — Modularização e Makefile

## Por que modularizar

Com tudo num arquivo só, o código fica longo, difícil de reaproveitar em outro programa e difícil de dividir em equipe. A solução é separar funções relacionadas em **módulos**.

## Um módulo = dois arquivos

| Arquivo | Contém |
|---|---|
| `modulo.h` | **declarações**: protótipos, tipos (`typedef struct`), constantes |
| `modulo.c` | **implementação** das funções; começa com `#include "modulo.h"` |

Quem quiser usar o módulo só faz `#include "modulo.h"`.

- `#include <stdio.h>` (sinais de menor/maior) → header do sistema.
- `#include "modulo.h"` (aspas) → header do nosso projeto.

## Include guard

Se um `.h` for incluído duas vezes, as definições aparecem em duplicidade e dá erro. Protege-se assim:

```c
#ifndef MODULO_H
#define MODULO_H
/* protótipos e tipos */
#endif
```

O pré-processador define `MODULO_H` na primeira vez; nas seguintes, pula até o `#endif`. O `mymemory.h` da professora já tem isso.

## Compilação separada

```bash
gcc -c mymemory.c          # compila só esse módulo -> mymemory.o (código-objeto)
gcc -c main.c              # -> main.o
gcc -o t2 main.o mymemory.o   # ligação (link): junta os .o e as bibliotecas -> executável
```

Vantagem: se só o `main.c` mudar, só ele é recompilado.

## Makefile

O `make` lê um roteiro (`Makefile`) com **alvos**, **dependências** e **comandos**, e só refaz o que ficou desatualizado:

```make
CFLAGS = -Wall -Wextra -g

t2: main.o mymemory.o
	gcc $(CFLAGS) -o t2 main.o mymemory.o

main.o: main.c mymemory.h
	gcc $(CFLAGS) -c main.c

mymemory.o: mymemory.c mymemory.h
	gcc $(CFLAGS) -c mymemory.c

clean:
	rm -f *.o t2
```

- A linha de comando começa obrigatoriamente com **TAB**, não com espaços.
- `CFLAGS` = opções do compilador: `-Wall`/`-Wextra` ligam os avisos, `-g` põe informação para o depurador.
- `make` constrói o primeiro alvo; `make clean` apaga os gerados.
- Colocar o `.h` nas dependências faz o `.o` ser refeito quando o header muda.

Alternativas ao make citadas em aula: CMake, SCons, autotools; as IDEs automatizam a mesma coisa.

## No T2

`mymemory.h` (interface da professora) + `mymemory.c` (nossa implementação) + `main.c` (testes) + `Makefile`. Critério do enunciado: **compilar sem erros nem avisos**, então o Makefile já liga `-Wall -Wextra` desde o primeiro dia.

---
Fontes: Moodle › Modularização e Criação de Bibliotecas — slides "Modularização em C" (Prof. Eduarda), "Modularização em C" (Prof. Marcelo Cohen, inf.pucrs.br/flash/progswb2/present/U01_progC/makefiles), "Criando bibliotecas estáticas e dinâmicas em C" (mflash.github.io/progswb2/aulas/libraries).
