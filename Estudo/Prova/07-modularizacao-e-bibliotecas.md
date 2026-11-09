# 07 — Modularização, Makefile e bibliotecas

## Por que modularizar

Tudo num arquivo só fica grande, difícil de reutilizar e de dividir em equipe.

## Os dois arquivos de um módulo

- **`.h` (cabeçalho)**: só **protótipos**, constantes e tipos. O "o quê".
- **`.c` (implementação)**: o corpo das funções. Inclui o próprio `.h`.
- Quem usa o módulo inclui só o `.h`.

```c
// bibfunc.h
#ifndef BIBFUNC_H
#define BIBFUNC_H
int fatorial(int v);
#endif
```

- `#ifndef/#define/#endif` = **include guard**: impede que o `.h` seja incluído duas vezes (definições duplicadas).
- `#include <stdio.h>` → cabeçalho do sistema. `#include "bibfunc.h"` → nosso, procurado na pasta do projeto.

## Compilar

```
gcc main.c bibfunc.c -o main      # tudo de uma vez (recompila tudo sempre)
gcc -c bibfunc.c                  # só compila: gera bibfunc.o
gcc -c main.c                     # gera main.o
gcc main.o bibfunc.o -o main      # liga os .o: gera o executável
```

- `-c` = compila **sem ligar** → código objeto `.o`.
- Vantagem: se só `main.c` mudou, só ele é recompilado.

## Makefile

Roteiro de compilação: diz **dependências** e **comandos**. Roda com `make`.

```make
main: main.o bibfunc.o
	gcc -o main main.o bibfunc.o
main.o: main.c bibfunc.h
	gcc -c main.c
bibfunc.o: bibfunc.c bibfunc.h
	gcc -c bibfunc.c
clean:
	rm -f *.o main
```

- Formato: `alvo: dependências` e, na linha de baixo, o comando começando com **TAB**.
- O `make` só refaz um alvo se alguma dependência é **mais nova** que ele.
- `make` sem nome roda o **primeiro** alvo. `make clean` roda o alvo `clean`.
- Variáveis: `CFLAGS = -Wall`, usadas com `$(CFLAGS)`. `$@` = nome do alvo.

## Bibliotecas: estática x dinâmica

| | Estática (`.a`) | Dinâmica (`.so`) |
|---|---|---|
| Quando entra | copiada **para dentro** do executável na ligação | só uma **referência**; o SO carrega na execução |
| Executável | maior | menor |
| Atualizar a biblioteca | precisa recompilar o programa | não precisa |
| Memória | cada programa tem sua cópia | uma cópia **compartilhada** |
| Criar | `ar rcs libx.a a.o b.o` | `gcc -fPIC -shared -o libx.so a.o b.o` |

- Usar as duas: `gcc main.c -o main -L. -lx` (`-L.` = procura na pasta atual; `-lx` = `libx`).
- `-fPIC` = código independente de posição (pode ser carregado em qualquer endereço).
- A dinâmica precisa ser achada ao executar: `LD_LIBRARY_PATH`.

## Para procurar no YouTube

Módulo no Moodle: **Modularização e Criação de Bibliotecas**

- modularização em C arquivo .h e .c
- include guard ifndef define em C
- compilação separada gcc -c
- makefile tutorial em C
- biblioteca estática e dinâmica em C
