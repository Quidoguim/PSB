# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository purpose

This is a coursework repository for **Programação de Software Básico** (PUCRS/Escola Politécnica). It is not a software project with build/lint/test tooling — there is no source code to compile yet. The repo currently holds the assignment brief and the group's topic selection for Trabalho 1.

## Trabalho 1

Study, understand, and present a GNU utility and/or C standard library (GLIBC) function, focused on idioms and practices of professional C programming in systems software.

- **Chosen topic:** [`sort.c`](https://github.com/coreutils/coreutils/blob/master/src/sort.c) from GNU Coreutils (to be formally reserved on Moodle 08/09). Originally written by Mike Haertel (1988), currently maintained by Paul Eggert.
- **Deadline:** deliverable due 22/09.
- **Minimum scope:** 600 lines of original source (comments and blank lines count). `sort.c` is 5000+ lines, so the presentation should focus on a well-chosen subset of functions rather than the whole file.
- **Deliverable:** a video (max 10 minutes) hosted on a media-sharing platform (e.g. YouTube, Zoom), covering the evaluation criteria below. Both group members must participate and identify themselves before speaking.
- Full assignment text: [Trabalho1/T1-PSB.pdf](Trabalho1/T1-PSB.pdf); summary in [README.md](README.md).
- Worked example of the expected report format (professor's model, analysis of `echo.c` by Marco Mangan): [Trabalho1/Modelo-Relatorio-T1-PSB.pdf](Trabalho1/Modelo-Relatorio-T1-PSB.pdf).

### Código-fonte

[Trabalho1/sort.c](Trabalho1/sort.c) — 5154 linhas, baixado do GNU Coreutils na branch `master`, referência commit [`bff0e54`](https://github.com/coreutils/coreutils/commit/bff0e54) ("build: sort: explicitly tag libcrypto dependency", 06/09/2026).

Subconjunto escolhido para o relatório/apresentação (~740 linhas, fio narrativo único: buffer → extração de chave → comparação → dispatch; deixa de fora `main()`, 558 linhas, e a maquinaria de threads/merge, complexas demais pra um vídeo de 10 min):

| Função | Linhas | Motivo |
|---|---|---|
| `try_growbuf`/`maybe_growbuf` | 1802-1861 | truque: buffer dobra de tamanho (realloc amortizado) |
| `begfield` | 1862-1908 | aritmética de ponteiros — início do campo |
| `limfield` | 1909-2018 | aritmética de ponteiros — fim do campo, contador regressivo |
| `fillbuf` | 2019-2169 | leitura de entrada/EOF, monta tabela de linhas via ponteiros |
| `keycompare` | 2946-3140 | núcleo: comparação multi-chave, bloco denso |
| `compare` | 3141-3187 | desempate por linha inteira |
| `sort()` | 4315-4444 | dispatcher memória vs. arquivo temporário; contém o único `goto` do arquivo fora de `check()` (`sort.c:4418`, label `finish`) |

O arquivo inteiro só tem 2 `goto`: `sort.c:3272` (dentro de `check()`, fora do subconjunto) e `sort.c:4418` (dentro de `sort()`, no subconjunto — cobre o critério "desvio incondicional" sem precisar entrar em `main()`).

### Evaluation criteria (9 points total)

| Criterion | Points |
|---|---|
| Author history (career, publications, related institution/project) | 1 |
| Coding conventions (block alignment, margins, identifiers) | 1 |
| Pointer/array manipulation (pointer arithmetic) | 1 |
| Code block analysis (responsibilities of blocks/helper functions) | 2 |
| "C programmer tricks" (idioms that cut instructions/memory/cycles) | 2 |
| Academic reference related to the program or its authors | 1 |
| Usage example (execution, stack/heap memory use, debugger or diagram) | 1 |
| Static and dynamic UML (or similar) diagrams | 1 |
| Automated build/tests (make, libraries, etc.) | 1 |

Constraint: **C only** — no C++, C#, Objective-C, or similar. `echo` cannot be chosen (used as the disicpline's own worked example).

## Passo a passo para iniciar o desenvolvimento

O modelo de relatório (análise de `echo.c`, 8 páginas) mostra o formato esperado: Introdução, Idiomas identificados, Divisão em blocos, Dependências (tabela), Cenário principal com diagrama estático de arquivo/funções e diagrama dinâmico de fluxo, Conclusão, Referências, e anexo com o código-fonte completo. Usar essa estrutura como guia para o relatório de `sort.c`.

1. Confirmar a reserva de `sort.c` no Moodle (prazo 08/09).
2. ~~Baixar o código-fonte de `sort.c` e guardar uma cópia no repositório, com a data/commit de referência anotada~~ — feito, ver [Código-fonte](#código-fonte) acima.
3. Levantar o histórico dos autores: Mike Haertel (autor original, 1988) e Paul Eggert (mantenedor atual, também conhecido pelo tz database) — carreira, publicações, contribuições em outros projetos.
4. ~~Escolher o subconjunto de funções a apresentar~~ — feito, ver tabela em [Código-fonte](#código-fonte) acima.
5. Identificar convenções de codificação do projeto (alinhamento, margem, padrão de identificadores GNU).
6. Mapear ocorrências de aritmética de ponteiros no subconjunto escolhido.
7. Levantar os "truques de programador C" (idiomas que reduzem instruções/memória/ciclos) presentes no trecho.
8. Dividir o subconjunto escolhido em blocos de responsabilidade e funções auxiliares, com tabela de dependências internas/externas (como a Tabela 1/2 do modelo).
9. Montar o diagrama estático (arquivo/funções e bibliotecas, como a Figura 1 do modelo) e o diagrama dinâmico (fluxo de execução, como a Figura 2).
10. Buscar ao menos uma referência acadêmica relacionada ao programa ou aos autores.
11. Preparar um exemplo de uso do programa (execução real, consumo de stack/heap) com depurador (gdb) ou ferramenta equivalente.
12. Criar um `Makefile` (ou script equivalente) para compilar/testar o trecho escolhido — cobre o critério de construção e testes automatizados.
13. Escrever o relatório seguindo a estrutura do modelo.
14. Preparar os slides/roteiro e gravar o vídeo (até 10 min), com cada integrante se identificando antes de falar.
15. Publicar o vídeo em plataforma de compartilhamento de mídia e entregar o link até 22/09.

## Working in this repo

When asked to help with this assignment, keep in mind the deliverable is a *presentation/video*, not a standalone program: work here will typically mean preparing source excerpts, annotated code walkthroughs, diagrams (static/dynamic UML), build/test scaffolding (e.g. a `Makefile` for the chosen `sort.c` subset), and supporting material — not building a new application from scratch.
