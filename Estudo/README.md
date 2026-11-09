# Material de estudo — Programação de Software Básico

Resumos escritos por nós a partir do material da disciplina no Moodle (turma 320, 2026/2, Prof. Eduarda Rodrigues Monteiro) e das apresentações do Prof. Marcelo Cohen linkadas lá. Servem para o T2 e para a prova.

São **resumos com nossas palavras e exemplos curtos**, não cópia dos slides. Os originais ficam no Moodle (link de cada fonte no fim de cada arquivo).

| # | Assunto | Seção do Moodle | Usado no T2 em |
|---|---|---|---|
| 01 | [Funções e passagem de parâmetros](01-passagem-de-parametros.md) | Passagem de parâmetros | todas as funções recebem `mymemory_t *` |
| 02 | [Structs e typedef](02-structs.md) | Strings, structs e unions | `allocation_t`, `mymemory_t`, operador `->` |
| 03 | [Ponteiros e aritmética de ponteiros](03-ponteiros.md) | Ponteiros e alocação dinâmica | contas de endereço dentro do pool |
| 04 | [Alocação dinâmica (malloc/free)](04-alocacao-dinamica.md) | Ponteiros e alocação dinâmica | pool e nós da lista |
| 05 | [Modularização e Makefile](05-modularizacao-e-makefile.md) | Modularização e Criação de Bibliotecas | `mymemory.h`/`.c`/`main.c`/`Makefile` |
| 06 | [Lista simplesmente encadeada](06-lista-encadeada.md) | Ponteiros e alocação dinâmica (exercício da lista) | lista de `allocation_t` |

**Para a prova (10/11):** ver [Prova/](Prova/README.md) — resumos curtos de todas as aulas (01 a 10), 48 V/F, 35 "o que imprime?" e 15 exercícios, com gabarito testado no compilador.

Seções do Moodle não resumidas nesta pasta (estão em `Prova/`): Primeiros Passos em C, Introdução à Linguagem C, Strings, Arquivos, Manipulação de bits. Os slides da Prof. Eduarda são PDFs que o Moodle só entrega como download. Os que forem salvos ficam em `slides-moodle/`, só local (fora do git, por ser material da PUCRS num repositório público), e são usados para complementar os resumos. Já incorporado: aula 10 (alocação dinâmica).
