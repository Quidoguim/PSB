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

### Histórico dos autores

**Mike Haertel**
- St. Olaf College (Minnesota). Verão de 1988: descobriu Emacs/GCC, escreveu para Richard Stallman e foi contratado pela FSF junto com o colega de faculdade Pete TerMaat ([GNU's Bulletin v1n5](https://www.gnu.org/bulletins/bull5.html#SEC7), fonte primária).
- Escreveu o motor de casamento de padrões original do GNU grep (Boyer-Moore + matchers DFA/kwset) — autor principal do grep do fim dos anos 1980 até os 2000. Na mesma leva, GNU diff e GNU sort (dezembro de 1988, data que bate com o cabeçalho do nosso `Trabalho1/sort.c`).
- **21/ago/2010:** postou na lista `freebsd-current` uma explicação hoje clássica de por que o GNU grep é rápido — fonte primária recuperada: [lists.freebsd.org/pipermail/freebsd-current/2010-August/019310.html](https://lists.freebsd.org/pipermail/freebsd-current/2010-August/019310.html). Ele descreve dois truques: (1) "avoids looking at every input byte" — lê em buffer grande e só localiza quebras de linha depois de achar um match, em vez de quebrar a entrada por linha; (2) executa menos de 3 instruções x86 por byte examinado, via Boyer-Moore com loop desenrolado; usa `mmap()` em vez de `read()` (ganho de >20% medido por ele). Frase dele: *"The key to making programs fast is to make them do practically nothing."*
- **Gancho pro relatório:** essa filosofia — fazer o mínimo de trabalho por byte — é exatamente o que aparece em `try_growbuf`/`begfield`/`limfield` no nosso subconjunto (buffer que dobra em vez de realocar a cada linha, varredura de campo via ponteiro sem cópia). Dois programas escritos independentemente, mesma cabeça de engenharia por trás.
- Depois migrou para arquitetura de CPU: Intel (projeto do Pentium 4), depois AMD, hoje de volta à Intel como CPU architect (LinkedIn/diretórios de indústria, consistente com BA em St. Olaf).
- Nota: um blog atribui a Haertel a co-arquitetura do AMD-V (virtualização da AMD) — sem fonte primária confiável, não citar no relatório sem confirmar.

**Paul Eggert**
- Nascido em 04/12/1954. Rice University (Engenharia Elétrica — Rice não tinha departamento de CS na época), formou-se em 1975. PhD em Ciência da Computação pela UCLA, 1980 ([página oficial UCLA Samueli](https://samueli.ucla.edu/people/paul-eggert/); perfil detalhado: [Rice Magazine, "The Time Zone Keeper", primavera de 2025](https://magazine.rice.edu/spring-2025/time-zone-keeper)).
- Deu aula na UC Santa Barbara por 3 anos, foi para a indústria (cofundou startups, trabalhou na System Development Corporation), depois foi CTO da Twin Sun Inc. (serviços técnicos em GNU/Linux/BSD para o mercado japonês). Hoje é Teaching Professor no Departamento de Ciência da Computação da UCLA.
- **Publicações**, além da manutenção de código: "File Systems in User Space" (USENIX Winter 1993, com Douglas Stott Parker Jr.); "Monte Carlo arithmetic: how to gamble with floating point and win" (*Computing in Science & Engineering*, 2000); "Perturbing and evaluating numerical programs without recompilation" (*Software: Practice and Experience*, 2005).
- **Padrões IETF:** coautor da [RFC 8536](https://www.rfc-editor.org/rfc/rfc8536) e [RFC 9636](https://www.rfc-editor.org/rfc/rfc9636) ("The Time Zone Information Format — TZif") e da RFC 6557 ("Procedures for Maintaining the Time Zone Database") — serve também como a referência acadêmica do critério 6 (passo 10 do roteiro).
- **tz database:** começou consertando entradas pontuais (Taiwan, Indonésia) no início dos anos 1990, depois de notar inconsistências fazendo negócios em fusos diferentes — "resolveu terminar o serviço" iniciado por Arthur David Olson. Editor oficial na IANA desde 2005, trabalho não remunerado, em paralelo às aulas. Método de pesquisa inclui almanaques de astrólogo, documentos legais, arquivos de governo e horários antigos de trem (ex.: Marrocos muda o horário no Ramadã por observação astronômica). Criou a convenção de nomes `America/New_York`.
- Outras contribuições GNU: Autoconf, Bison, Diffutils, GNU RCS (mantenedor desde 1989), gzip, Emacs, GCC, glibc, GNU tar e GNU Coreutils (co-autor/mantenedor de `sort.c` ao lado de Haertel).
- Reconhecimento: [FSF Award for the Advancement of Free Software (2021)](https://www.cs.ucla.edu/professor-paul-eggert-awarded-fsf-free-software-awards/), Lockheed Martin Excellence in Teaching Award (2012); "Time Zone King" na mídia (National Geographic, The Register, Rice Magazine).

Instituição/projeto em comum: ambos ligados ao **GNU Project / Free Software Foundation**; o código estudado vive hoje no guarda-chuva do **GNU Coreutils**.

### Convenções de codificação

Fonte normativa: [GNU Coding Standards, seção Formatting](https://www.gnu.org/prep/standards/html_node/Formatting.html) (citações diretas). Evidência levantada no subconjunto escolhido (linhas 1802-4444):

| Regra do padrão (citação direta) | Onde aparece no subconjunto |
|---|---|
| "keep the length of source lines to 79 characters or less" | linhas do subconjunto ficam em torno de 80 colunas |
| "put the open-brace that starts the body of a C function in column one" | `try_growbuf` (`sort.c:1802-1804`), `begfield`, `keycompare`, `sort()` — tipo de retorno numa linha, assinatura na seguinte, `{` sozinha na coluna 0 |
| "spaces before the open-parentheses and after the commas" | `malloc (alloc)` (`sort.c:1808`), `memcpy (newbuf, oldbuf, buf->used)` (`sort.c:1816`) — contraste útil pro vídeo: K&R/Linux kernel não usam esse espaço em chamadas |
| "split it before an operator, not after one" | `sort.c:3007-3009` — ternário quebrado com `?`/`:` no início da linha de continuação, dentro de `keycompare` |
| template `do / { ... } / while (cond);` | `sort.c:3090-3098` — do-while real dentro de `keycompare`, indentação idêntica ao exemplo do padrão |
| "whatever style you use, please use it consistently" | mesmo estilo do início ao fim, ~2600 linhas entre `try_growbuf` e `sort()` |

Achado extra em `keycompare` (`sort.c:3052-3074`): a macro `CMP_WITH_IGNORE` usa o idioma clássico `do { ... } while (0)` pra fazer uma macro multi-statement se comportar como um único comando — documenta convenção de macro e já é candidato a "truque" (passo 7).

Demais pontos:
- **Indentação:** 2 espaços por nível, sem tabs no código — tabs só alinham o `\` de continuação em macros multi-linha (ex. `CMP_WITH_IGNORE`, `sort.c:3052-3061`).
- **Ponteiros:** `*` colado ao identificador, não ao tipo — `char *ptr`, `struct line *line` (`sort.c:1865`).
- **Identificadores:** `snake_case` para funções e variáveis (`try_growbuf`, `begfield`, `new_linelim`); `MAIÚSCULO` para macros/atributos (`ATTRIBUTE_PURE`, `TAB_DEFAULT`); `struct nome` sempre minúsculo.
- **Comentários:** bloco `/* ... */` antes da função descrevendo o que ela faz; nomes de parâmetros citados em MAIÚSCULO dentro do texto — ex. "Try to grow BUF according to POLICY" antes de `maybe_growbuf` (`sort.c:1838`).

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

1. ~~Confirmar a reserva de `sort.c` no Moodle~~ — feito, reserva formal concluída dentro do prazo (08/09).
2. ~~Baixar o código-fonte de `sort.c` e guardar uma cópia no repositório, com a data/commit de referência anotada~~ — feito, ver [Código-fonte](#código-fonte) acima.
3. ~~Levantar o histórico dos autores~~ — feito, ver [Histórico dos autores](#histórico-dos-autores) acima.
4. ~~Escolher o subconjunto de funções a apresentar~~ — feito, ver tabela em [Código-fonte](#código-fonte) acima.
5. ~~Identificar convenções de codificação do projeto~~ — feito, ver [Convenções de codificação](#convenções-de-codificação) acima.
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
