# Trabalho 1 – Programação de Software Básico

Repositório dedicado ao Trabalho 1 da disciplina de Programação de Software Básico (PUCRS/Escola Politécnica). O trabalho consiste no estudo, compreensão e apresentação de um utilitário do GNU e/ou de funções da biblioteca padrão da linguagem C (conforme implementada no GNU C), com foco nos idiomas e práticas típicas da programação profissional em C no contexto de software básico.

## Objetivo

Conhecer e apresentar idiomas e práticas da programação em **linguagem C** no contexto de **software básico** (sistemas operacionais, compiladores, utilitários e bibliotecas).

> Atenção: não são aceitos trabalhos sobre programas escritos em C++, C#, Objective-C ou similares.

## Enunciado (resumo)

- A dupla deve escolher um utilitário do GNU (ex.: [GNU Coreutils](https://github.com/coreutils/coreutils/tree/master/src), [GNU Savannah](https://git.savannah.gnu.org/cgit/)) e/ou funções da biblioteca padrão de C ([GLIBC](https://github.com/lattera/glibc)).
- O código-fonte estudado deve cobrir **no mínimo 600 linhas** (incluindo comentários e linhas em branco), no texto original do repositório escolhido. Se o programa escolhido tiver menos linhas, devem ser incluídos programas/funções adicionais até atingir o mínimo.
- O utilitário `echo` **não pode ser reservado** (é usado como exemplo no relatório disponibilizado pela disciplina).

## Modalidade do Trabalho

- Trabalho em **duplas** (máximo dois integrantes).
- Reserva prévia do programa/função de biblioteca escolhido, feita na área indicada no Moodle.
- Apenas **uma reserva por programa/função principal** — em caso de duplicidade, vale a primeira reserva registrada.
- Após a data de reserva, não é mais possível reservar novos programas, apenas ajustar os já alocados.

## Cronograma

| Etapa | Data |
|---|---|
| Escolha de grupos e programas | 08/09 |
| Entrega | 22/09 |

**Programa/utilitário escolhido:** [`sort.c`](https://github.com/coreutils/coreutils/blob/master/src/sort.c) (GNU Coreutils) — reserva formal feita no Moodle dentro do prazo (08/09).

`sort.c` foi escrito originalmente por Mike Haertel (dezembro de 1988) e é mantido hoje por Paul Eggert — ambos com bastante material disponível para o critério de "Histórico dos autores" (Eggert também é conhecido pela manutenção do banco de dados de fusos horários, o tz database). É um dos utilitários mais extensos do GNU Coreutils (mais de 5000 linhas), o que dá bastante espaço para os critérios de "Análise de blocos" e "Truques" (implementa merge sort externo, comparação por múltiplas chaves, otimizações de I/O e paralelismo) — vale a pena focar a apresentação em um subconjunto bem escolhido de funções, já que cobrir o arquivo inteiro em 10 minutos de vídeo não é viável.

`cat`, `wc`, `head` e `uniq.c` já haviam sido reservados por outras duplas.

## Progresso

- [x] Dupla formada, tema escolhido: `sort.c`
- [x] Reserva formal confirmada no Moodle (feita dentro do prazo, 08/09)
- [x] Código-fonte baixado e versionado — [Trabalho1/sort.c](./Trabalho1/sort.c), commit de referência [`bff0e54`](https://github.com/coreutils/coreutils/commit/bff0e54) do GNU Coreutils (branch `master`, 06/09/2026)
- [x] Subconjunto de funções definido para o relatório/apresentação (ver abaixo)
- [x] Histórico dos autores (Mike Haertel, Paul Eggert) levantado
- [x] Convenções de codificação identificadas
- [x] Aritmética de ponteiros mapeada no subconjunto
- [x] Truques de programador C levantados
- [x] Blocos de responsabilidade e dependências divididos
- [x] Diagramas estático e dinâmico
- [x] Referência acadêmica levantada
- [x] Exemplo de uso com execução real (stack/heap)
- [x] Makefile / testes automatizados (12/12 passando)
- [ ] Relatório escrito (esqueleto `.docx` gerado a partir deste repositório quando o levantamento estiver completo, pra revisão da dupla)
- [ ] Vídeo gravado e publicado (prazo 22/09)

### Subconjunto de código selecionado

~677 linhas de `sort.c`, cobrindo um fluxo único (buffer → extração de chave → comparação → dispatch). Fora do escopo: `main()` (558 linhas) e a maquinaria de threads/merge.

| Função | Linhas | Motivo |
|---|---|---|
| `try_growbuf`/`maybe_growbuf` | 1802-1857 | truque: buffer dobra de tamanho (realloc amortizado) |
| `begfield` | 1862-1903 | aritmética de ponteiros — início do campo |
| `limfield` | 1908-2010 | aritmética de ponteiros — fim do campo, contador regressivo |
| `fillbuf` | 2018-2133 | leitura de entrada/EOF, monta tabela de linhas via ponteiros |
| `keycompare` | 2946-3136 | núcleo: comparação multi-chave |
| `compare` | 3141-3181 | desempate por linha inteira |
| `sort()` | 4314-4441 | dispatcher memória vs. arquivo temporário; contém o único `goto` do arquivo fora de `check()` |

Detalhes e raciocínio completo: [CLAUDE.md](./CLAUDE.md#código-fonte).

### Histórico dos autores

**Mike Haertel** (St. Olaf College) foi contratado pela FSF em 1988 e escreveu o GNU grep, GNU diff e GNU sort. Em 2010 publicou uma explicação hoje clássica de por que o grep é rápido ("the key to making programs fast is to make them do practically nothing") — a mesma filosofia aparece nos truques de buffer/ponteiro do nosso subconjunto de `sort.c`. Depois seguiu carreira em arquitetura de CPU (Intel — Pentium 4 —, depois AMD, hoje Intel de novo).

**Paul Eggert** (Rice University, PhD UCLA 1980) é o mantenedor atual de `sort.c`. Publicou em USENIX (1993), *Computing in Science & Engineering* (2000) e *Software: Practice and Experience* (2005), e é coautor das RFCs 8536/9636/6557 sobre o formato do tz database, que mantém na IANA desde 2005. Contribui também com Autoconf, Diffutils, RCS, gzip, GCC, glibc e GNU Coreutils; recebeu o FSF Award for the Advancement of Free Software em 2021.

Fontes e detalhes completos: [CLAUDE.md](./CLAUDE.md#histórico-dos-autores).

### Convenções de codificação

Confrontamos o subconjunto direto com o [GNU Coding Standards](https://www.gnu.org/prep/standards/html_node/Formatting.html) oficial: a regra "open-brace ... in column one" bate com `try_growbuf`/`begfield`/`sort()`, "spaces before the open-parentheses" bate com `malloc (alloc)`, "split it before an operator, not after one" bate com o ternário de `keycompare` (`sort.c:3007-3009`), e o template `do {...} while (cond)` aparece igual em `sort.c:3090-3098`. Também: 2 espaços de indentação (sem tabs no código), `*` de ponteiro colado à variável (`char *ptr`), identificadores em `snake_case` e macros em MAIÚSCULO, margem em torno de 80 colunas.

Detalhes e exemplos com número de linha: [CLAUDE.md](./CLAUDE.md#convenções-de-codificação).

### Aritmética de ponteiros

O `struct buffer` de `sort.c` guarda, numa única alocação, o texto das linhas crescendo do início pro fim e a tabela de linhas crescendo do fim pro início — encontrando no meio. Toda a leitura/escrita usa ponteiro (soma, subtração, incremento/decremento), nunca índice contado à parte: `fillbuf` decrementa o ponteiro da tabela a cada linha nova (`line--`, `sort.c:2080`), usa subtração de ponteiros pra tamanho de linha (`ptr - line_start`) e espaço livre; `begfield`/`limfield` andam com `++ptr`/`ptr += n`; `sort()` reusa a mesma tabela "de trás pra frente" e percorre `files` como array de ponteiros (`*files`, depois `files++`).

Detalhes com número de linha: [CLAUDE.md](./CLAUDE.md#aritmética-de-ponteiros).

### Truques de programador C

Oito achados com linha exata: tabelas indexadas por byte em vez de comparação em cadeia (`blanks[to_uchar (*ptr)]`, protegido contra sign-extension pelo `to_uchar()` do `system.h` do coreutils); contagem regressiva embutida em `while (ptr < lim && eword--)`; crescimento geométrico (triplica) do buffer em `maybe_growbuf`; small-buffer optimization com `char stackbuf[4000]` em `keycompare` pra evitar `malloc` no caso comum; terminação NUL temporária in-place na mesma função; macro segura com `do { ... } while (0)` em `CMP_WITH_IGNORE`; `ATTRIBUTE_PURE` em `limfield` (atributo `pure` do GCC, habilita eliminação de subexpressão comum); e a tabela de linhas bidirecional já descrita acima.

Detalhes com número de linha e fontes: [CLAUDE.md](./CLAUDE.md#truques-de-programador-c).

### Blocos de responsabilidade e dependências

Cada uma das 7 funções quebrada em sub-blocos (tipo Tabela 2 do modelo — ex.: `fillbuf` = reaproveita sobra + laço de leitura + trata EOF/cresce buffer) e tabela de dependências internas/externas (tipo Tabela 1): libc (`malloc`, `memchr`, `fread`...), gnulib/coreutils (`to_uchar`, `xmemcoll0`, `filenvercmp`...) e funções do próprio `sort.c` fora do subconjunto — inclusive a maquinaria de threads/merge que `sort()` chama mas a gente não detalha (`sortlines`, `merge`, `queue_*`).

Tabelas completas com número de linha: [CLAUDE.md](./CLAUDE.md#blocos-de-responsabilidade-e-dependências).

### Diagramas estático e dinâmico

Desenhados à mão em SVG, preto e branco, estilo yUML igual ao modelo — sem gerador automático:

- **Estático** — [diagrama-estatico-sort.svg](./Trabalho1/diagramas/diagrama-estatico-sort.svg): grafo de chamadas entre as 7 funções; `begfield()`/`limfield()` aparecem uma vez só, recebendo seta de `fillbuf()` (leitura) e de `keycompare()` (comparação) — o "X" no meio é o mesmo achado da seção de Aritmética de ponteiros.
- **Dinâmico** — [diagrama-dinamico-sort.svg](./Trabalho1/diagramas/diagrama-dinamico-sort.svg): diagrama de atividades UML do fluxo real de `sort()`, com as decisões (`-k`? mais de 1 linha? `nthreads>1`? há mais dados? gravou direto?) e o laço de volta até esgotar a entrada. Caixas de borda grossa marcam onde o subconjunto entra na execução; caixas tracejadas marcam o que fica de fora (`sortlines`, `merge`).

Explicação completa: [CLAUDE.md](./CLAUDE.md#diagramas-estático-e-dinâmico).

### Referências acadêmicas

4 referências, cada uma com um papel (padrão do modelo, que usou 3):

1. Knuth, *The Art of Computer Programming, Vol. 3: Sorting and Searching*, 2ª ed. (1998), Seção 5.4 — o algoritmo que `sort()` implementa.
2. Eggert & Parker, "File Systems in User Space", USENIX Winter 1993 — autor, sistemas.
3. Eggert et al., RFC 8536, "The Time Zone Information Format (TZif)" — autor, padronização.
4. Mayrhauser & Vans (1995), "Program comprehension during software maintenance and evolution", *Computer* 28 — metodologia (mesma do modelo do `echo.c`), embasa a divisão em blocos.

Citações completas: [CLAUDE.md](./CLAUDE.md#referência-acadêmica).

**Nota:** [mflash/DevCPP](https://github.com/mflash/DevCPP) (achado no Moodle) é um template de ambiente C/C++, não obra acadêmica — não entra nessa lista, mas é candidato a base pro `Makefile`/teste do passo 12. Ver [CLAUDE.md](./CLAUDE.md#ambiente-de-buildteste-nota-para-o-passo-12).

### Exemplo de uso (execução, stack/heap)

[harness_subconjunto.c](./Trabalho1/exemplo-uso/harness_subconjunto.c) — cópia verbatim de 5 funções do subconjunto (`try_growbuf`, `maybe_growbuf`, `begfield`, `limfield`, `buffer_linelim`) compiladas isoladas com stubs mínimos, já que `sort.c` completo não compila sem a árvore de build do coreutils. gdb local ficou bloqueado pelo ambiente (mesmo bloqueio de rede do `pacman`) — rodamos num compilador online real (gcc 13.2.0) em vez de prever a saída. [Saída real](./Trabalho1/exemplo-uso/saida-exemplo.txt): buffer de heap crescendo `64→224→704→2144→5024` bytes (bate com o crescimento ×3 documentado em Truques), endereços de pilha reais em `begfield`/`limfield`, e um achado não planejado — o campo extraído veio com o separador incluído (`" 42"`), comportamento documentado no próprio código, não bug.

Detalhes: [CLAUDE.md](./CLAUDE.md#exemplo-de-uso-execução-stackheap).

### Construção e testes automatizados

O enunciado não fixa uma quantidade mínima de testes — estipulamos a nossa: **12 testes** (1 caminho normal + 1 borda por dupla de funções com lógica condicional), cobrindo as 6 funções do subconjunto. Sem framework externo, `exit(1)` se algo falhar (pensado pra `make test`, não conferência visual).

- [subconjunto.h](./Trabalho1/exemplo-uso/subconjunto.h) — as 6 funções + stubs, compartilhado entre demo e testes.
- [testes.c](./Trabalho1/exemplo-uso/testes.c) · [Makefile](./Trabalho1/exemplo-uso/Makefile) (`all`/`run`/`test`/`clean` — Makefile plano, não o CMake do DevCPP, porque o critério pede "make") · [saída real](./Trabalho1/exemplo-uso/saida-testes.txt): **12/12 passando**.

**Achado real** (não decoração): na primeira rodada deu 11/12 — o teste estava errado, não o `sort.c`. `try_growbuf` compara o alloc **já alinhado** por `line_aligned_size`, que sempre aumenta o valor em pelo menos 1 byte mesmo quando já é múltiplo do alinhamento. Corrigido; histórico completo no arquivo de saída.

Raciocínio completo: [CLAUDE.md](./CLAUDE.md#construção-e-testes-automatizados).

## Entrega

- **Relatório escrito** + **vídeo de até 10 minutos** apresentando os critérios de avaliação abaixo (ver [Progresso](#progresso)).
- Link do vídeo hospedado em sistema de compartilhamento de mídia (ex.: Zoom, YouTube).
- Participação de **todos os integrantes da dupla é obrigatória** no vídeo; ausência gera desconto de 30% na nota.
- Cada participante deve se identificar antes de falar e, se possível, manter a webcam ligada.
- A responsabilidade pelo envio e reprodução do vídeo (link válido, sem falhas de áudio/imagem) é dos autores — falhas nesse sentido resultam em trabalho não avaliado.

## Critérios de Avaliação

| Item | Pontuação |
|---|---|
| Histórico dos autores do código, instituição/projeto relacionado | 1 ponto |
| Convenções de codificação (alinhamento, margem, identificadores) | 1 ponto |
| Manipulação de ponteiros e vetores (aritmética de ponteiros) | 1 ponto |
| Análise de blocos de código e funções auxiliares | 2 pontos |
| Truques característicos da linguagem C ("coisas de programador C") | 2 pontos |
| Referência acadêmica relacionada ao programa/autores | 1 ponto |
| Exemplo de uso do programa (execução, consumo de memória, depurador/diagrama) | 1 ponto |
| Diagrama estático e dinâmico (UML ou notação similar) | 1 ponto |
| Construção e testes automatizados (make, bibliotecas etc.) | 1 ponto |

## Documento original

O enunciado completo está disponível em [`T1-PSB.pdf`](./Trabalho1/T1-PSB.pdf).

## Referências

- [1] GNU Core Utils: https://github.com/coreutils/coreutils/tree/master/src
- [2] GNU Savannah: https://git.savannah.gnu.org/cgit/
- [3] GLIBC: https://github.com/lattera/glibc
