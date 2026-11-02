# Uso de inteligência artificial no Trabalho 2

Integrantes: ____________________ (preencher)

Usamos o assistente Claude (Anthropic), via Claude Code, como apoio durante o trabalho:

- **Entendimento do enunciado:** pedimos uma explicação do problema (pool, lista de alocações, estratégias de alocação, fragmentação) e uma lista das inconsistências do código-base. O `main.c` fornecido chamava `mymemory_malloc`/`mymemory_release`, que não existem no `mymemory.h`.
- **Planejamento:** a IA propôs uma divisão em etapas (init/cleanup, display, alloc, free, stats, testes). As decisões de projeto foram discutidas e aceitas pelo grupo, com a orientação de manter o código simples e usar apenas o conteúdo visto em aula (ponteiros, alocação dinâmica, structs, lista encadeada, modularização e Makefile):
  - First Fit;
  - lista ordenada por endereço;
  - contas de endereço com `char *`;
  - alinhamento a 8 bytes.
- **Implementação:** a IA escreveu o código de `mymemory.c`, `main.c` e `Makefile` seguindo essas decisões. O grupo leu, executou e conferiu o código.
- **Explicação:** a IA preparou explicações de cada escolha relacionando-as com o material da disciplina, que usamos para estudar o código e preparar a apresentação.

**Verificações feitas pelo grupo:**
- compilação com `-Wall -Wextra`, sem avisos;
- execução dos testes do `main.c`, com 22 verificações OK;
- checagem de vazamento com `leaks --atExit`, com 0 vazamentos.

Nenhum código foi copiado da internet.
