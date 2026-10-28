# Programação de Software Básico

Repositório da disciplina de Programação de Software Básico (PUCRS/Escola Politécnica), em C.

## Trabalho 1 — análise de `sort.c` (GNU Coreutils)

**Status: concluído.** Relatório escrito, vídeo gravado e publicado, entrega realizada.

Estudo de um subconjunto de [`sort.c`](./Trabalho1/sort.c) (~677 linhas: `try_growbuf`/`maybe_growbuf`, `begfield`, `limfield`, `fillbuf`, `keycompare`, `compare`, `sort()`), com foco nos idiomas e práticas de programação profissional em C. Código baseado no commit [`bff0e54`](https://github.com/coreutils/coreutils/commit/bff0e54) do GNU Coreutils.

- **Relatório:** [Trabalho1/relatorio/Relatorio-T1-sort.c.docx](./Trabalho1/relatorio/Relatorio-T1-sort.c.docx)
- **Diagramas:** [estático](./Trabalho1/diagramas/diagrama-estatico-sort.svg) · [dinâmico](./Trabalho1/diagramas/diagrama-dinamico-sort.svg)
- **Exemplo de uso e testes:** [Trabalho1/exemplo-uso/](./Trabalho1/exemplo-uso/) (`make test` — 12/12 passando)
- **Enunciado:** [Trabalho1/T1-PSB.pdf](./Trabalho1/T1-PSB.pdf)

Notas detalhadas de cada etapa (autores, convenções, ponteiros, truques, blocos, referências): [CLAUDE.md](./CLAUDE.md).

## Trabalho 2 — gerenciador de memória (memory pool)

**Status: enunciado recebido (27/10), desenvolvimento não iniciado.**

Biblioteca em C que aloca um único bloco de memória (o *pool*) e, a partir dele, atende pedidos de alocação e liberação, controlando as regiões ocupadas com uma lista encadeada. `malloc`/`free` só podem ser usados para o pool e para as estruturas de controle (`mymemory_t` e os nós `allocation_t`).

- **Funções:** `mymemory_init`, `mymemory_alloc`, `mymemory_free`, `mymemory_display`, `mymemory_stats`, `mymemory_cleanup`.
- **Estratégias de alocação:** First Fit, Best Fit ou Worst Fit; teste com ~50 blocos de tamanhos variados.
- **Avaliação:** compilar sem erros nem avisos, funcionar conforme especificado, pouca fragmentação, sem vazamento de memória, e apresentação em sala (~10 min) explicando o código em detalhe. Grupos de até 3; entrega do código-fonte em `.tar.gz`/`.zip` pelo Moodle.
- **Código-base da professora:** [Trabalho2/codigo-base/](./Trabalho2/codigo-base/) (`main.c`, `mymemory.h`).

## Exercício — lista encadeada (alocação dinâmica)

[ListaEncadeada/lista.c](./ListaEncadeada/lista.c): lista simplesmente encadeada com `add`, `addStart`, `removeByValue`, `show`, `freeList` e menu interativo.
