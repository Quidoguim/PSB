# Makefile do subconjunto estudado de sort.c (passo 12 do roteiro,
# CLAUDE.md). Compila o exemplo de uso (demo) e os testes automatizados
# (testes) do mesmo jeito que "make"/gcc puro -- sem CMake, pra bater
# direto com o critério "construção e testes automatizados (make,
# bibliotecas etc.)". mflash/DevCPP (ambiente C/C++ da disciplina, achado
# no Moodle) foi a referência de toolchain (gcc), mas usamos um Makefile
# simples em vez do CMake dele porque o critério nomeia "make" e um
# Makefile plano é mais fácil de ler/avaliar num relatório.
#
# Alvos:
#   make            -> compila demo e testes
#   make run         -> compila e roda a demonstração (passo 11)
#   make test        -> compila e roda os testes; falha (exit != 0) se
#                        qualquer teste falhar -- pensado pra CI/correção
#                        automática, não pra conferência visual
#   make clean       -> remove os binários

CC      = gcc
CFLAGS  = -std=gnu17 -O0 -g -Wall -Wextra
DEPS    = subconjunto.h

.PHONY: all run test clean

all: demo testes

demo: harness_subconjunto.c $(DEPS)
	$(CC) $(CFLAGS) -o demo harness_subconjunto.c

testes: testes.c $(DEPS)
	$(CC) $(CFLAGS) -o testes testes.c

run: demo
	./demo

test: testes
	./testes

clean:
	rm -f demo testes demo.exe testes.exe
