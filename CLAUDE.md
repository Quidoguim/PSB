# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository purpose

This is a coursework repository for **Programação de Software Básico** (PUCRS/Escola Politécnica). It is not a software project with build/lint/test tooling — there is no source code to compile yet. The repo currently holds the assignment brief and the group's topic selection for Trabalho 1.

## Trabalho 1

Study, understand, and present a GNU utility and/or C standard library (GLIBC) function, focused on idioms and practices of professional C programming in systems software.

- **Chosen topic:** [`sort.c`](https://github.com/coreutils/coreutils/blob/master/src/sort.c) from GNU Coreutils (to be formally reserved on Moodle 08/09). Originally written by Mike Haertel (1988), currently maintained by Paul Eggert.
- **Deadline:** deliverable due 22/09.
- **Minimum scope:** 600 lines of original source (comments and blank lines count). `sort.c` is 5000+ lines, so the presentation should focus on a well-chosen subset of functions rather than the whole file.
- **Deliverable:** two parts — (1) a written report and (2) a video (max 10 minutes) hosted on a media-sharing platform (e.g. YouTube, Zoom) presenting it, covering the evaluation criteria below. Both group members must participate in the video and identify themselves before speaking.
- Full assignment text: [Trabalho1/T1-PSB.pdf](Trabalho1/T1-PSB.pdf); summary in [README.md](README.md).
- Worked example of the expected report format (professor's model, analysis of `echo.c` by Marco Mangan): [Trabalho1/Modelo-Relatorio-T1-PSB.pdf](Trabalho1/Modelo-Relatorio-T1-PSB.pdf). Use its structure (Introdução, Idiomas, Divisão em blocos, Dependências, Cenário principal, diagramas, Conclusão, Referências, anexo com código) as the skeleton for the `sort.c` report.
- **Report workflow:** every section below (Código-fonte, Histórico dos autores, Convenções de codificação, Aritmética de ponteiros, etc.) is the running source of truth for the report's content — keep it here as each roadmap step completes. Once the group signals the content is complete, generate a `.docx` skeleton (via the `docx` skill) following the model's structure, populated from these sections, for the user to review and complete — do not generate it earlier than that unless asked.

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

### Aritmética de ponteiros

**Insight estrutural único que amarra o subconjunto inteiro:** o `struct buffer` de `sort.c` guarda, numa única alocação, o texto das linhas crescendo do início pro fim e a tabela `struct line[]` crescendo do fim pro início — os dois lados se encontram no meio. Toda leitura/escrita nessa estrutura é feita com aritmética de ponteiros (incremento, decremento, subtração), nunca com um índice contado à parte. É o mesmo motivo de `keylist`/`begfield`/`limfield` nunca indexarem strings com `[i]`: um ponteiro guarda a posição, avança sozinho, e a subtração de dois ponteiros já dá o tamanho — sem `strlen`, sem laço extra.

**`begfield` (`sort.c:1862-1903`)**
- `char *ptr = line->text, *lim = ptr + line->length - 1;` (`1865`) — soma ponteiro+inteiro pra montar o sentinela de fim sem segundo laço.
- `++ptr` nos quatro `while` (`1878, 1884, 1886, 1893`) — o próprio ponteiro é o contador do laço.
- `size_t remaining_bytes = lim - ptr;` (`1896`) — subtração de ponteiros dá a distância restante direto, sem recalcular.
- `ptr += schar;` (`1898`) — soma ponteiro+inteiro pra pular SCHAR bytes de uma vez.

**`limfield` (`sort.c:1909-2009`)** — espelha o mesmo idioma pro fim do campo: `ptr + line->length - 1` (`1912`), `lim - ptr` (`1928`, `2002`), `ptr += echar` (`2004`).

**`fillbuf` (`sort.c:2019-2132`)** — onde a aritmética de ponteiros é mais densa:
- `char *ptr = buf->buf + buf->used;` (`2039`) — retoma a escrita exatamente onde a última leitura parou, sem guardar posição em variável separada.
- `size_t avail = (char *) linelim - buf->nlines * line_bytes - ptr;` (`2042`) — uma linha só mistura subtração de ponteiros, cast e multiplicação pra calcular o espaço livre entre a região de texto (crescendo pra frente) e a tabela de linhas (crescendo pra trás).
- `char *line_start = buf->nlines ? line->text + line->length : buf->buf;` (`2043`) — ponteiro + campo acha o início da próxima linha.
- `char *ptrlim = ptr + bytes_read;` (`2054`) e `ptrlim[-1]` (`2067`) — índice negativo sobre ponteiro = "o último byte lido", sem variável extra.
- `ptr = p + 1;` (`2079`) — avança pro byte seguinte ao delimitador achado por `memchr`.
- `line--;` (`2080`) — **a tabela de linhas é preenchida de trás pra frente**: cada linha nova decrementa o ponteiro em vez de incrementar um índice. É a prova direta do "os dois lados se encontram no meio".
- `line->length = ptr - line_start;` (`2082`) — subtração de ponteiros = tamanho da linha, sem `strlen`.
- `buf->used = ptr - buf->buf;` (`2113`) e `buf->left = ptr - line_start;` (`2119`) — ponteiro absoluto convertido de volta pra offset, pra guardar em `struct buffer`.
- `buf->nlines = buffer_linelim (buf) - line;` (`2114`) — subtração entre dois `struct line *` (não `char *`) conta quantas linhas foram preenchidas nesta leitura.

**`keycompare` (`sort.c:2946-3136`)** — consome os ponteiros já calculados por `begfield`/`limfield`/`fillbuf`: `size_t lena = lima - texta;` (`2970`) repete o idioma "subtração de ponteiro = tamanho". Também usa a string como buffer mutável: `ta[tlena] = '\0';` seguido de restauração (`enda`/`endb`) — trata o ponteiro de texto como array temporário sem alocar cópia.

**`sort()` (`sort.c:4315-4441`)**
- `char const *file = *files;` (`4330`) seguido de `files++;` (`4357`) — percorre o array de ponteiros `char *const *files` (mesma forma de `argv`) incrementando o próprio ponteiro, em vez de indexar `files[i]`.
- `line - buf.nlines` (`4407`), `line - 1` (`4413`), `line - i - 1` (`4409`) — reusa a mesma tabela "de trás pra frente" montada em `fillbuf`, confirmando que não é um truque isolado, é um padrão estrutural do arquivo inteiro.

### Truques de programador C

**1. Tabelas indexadas por byte em vez de comparação em cadeia** — `#define UCHAR_LIM (UCHAR_MAX + 1)` (`sort.c:83`), `static bool blanks[UCHAR_LIM];` (`sort.c:270`), `static char const unit_order[UCHAR_LIM] = {...}` (`sort.c:2136-2164`). `blanks[to_uchar (*ptr)]` (usado em `begfield`/`limfield`, ex. `sort.c:1883,1892,1936`) vira **uma leitura de memória** em vez de comparar `*ptr` contra espaço/tab um a um — clássica troca de branches por lookup table, exatamente o que o critério pede ("reduz número de instruções geradas/executadas").
   - Depende de `to_uchar()`, definido em `system.h` do coreutils como `static inline unsigned char to_uchar (char ch) { return ch; }`, com o comentário original: *"Convert a possibly-signed character to an unsigned character. This is a bit safer than casting to unsigned char, since it catches some type errors that the cast doesn't."* ([fonte: system.h, coreutils 8.23](http://agentzh.org/misc/code/coreutils/system.h.html)). Sem essa conversão, um byte ≥ 128 num `char` signed vira índice **negativo** ao indexar `blanks[]` — comportamento indefinido/corrupção de memória. É um dos erros clássicos de programador C iniciante que o código evita sistematicamente.

**2. Contagem regressiva embutida na condição do laço** — `while (ptr < lim && eword--)` (`sort.c:1926,1934`), `while (ptr < lim && sword--)` (`sort.c:1873,1881`). O decremento acontece dentro da própria condição do `while`, avaliado por curto-circuito só quando `ptr < lim` já é verdade — dispensa uma linha e uma variável de controle separada. É literalmente o idioma "contagem regressiva" citado por nome no enunciado da disciplina.

**3. Crescimento geométrico (amortizado) do buffer** — `maybe_growbuf` (`sort.c:1840-1857`): `if (buf->alloc <= policy->limit / 3) alloc = buf->alloc * 3;` — triplica o tamanho do buffer em vez de crescer sob demanda linha a linha. `realloc` a cada poucas linhas em vez de a cada linha muda o custo total de O(n²) pra O(n) amortizado — a mesma ideia por trás do crescimento de um `std::vector` (C++) ou de uma lista do Python, aqui implementada à mão em C.

**4. Small-buffer optimization (evita heap no caso comum)** — `keycompare` (`sort.c:2985-3000`): `char stackbuf[4000];` seguido de `if (size <= sizeof stackbuf) ta = stackbuf; else ta = allocated = xmalloc (size);`. Só aloca no heap quando a chave de ordenação não cabe nos 4000 bytes da pilha — no caso comum (linhas curtas), zero `malloc`/`free` por comparação, numa função chamada uma vez por par de linhas no sort inteiro.

**5. Terminação NUL temporária, in-place** — `keycompare` (`sort.c:2981-3046`): `char enda = ta[tlena]; ... ta[tlena] = '\0'; ... ta[tlena] = enda;`. Toma emprestado 1 byte do próprio buffer compartilhado pra terminar a "string" temporariamente (funções como `xmemcoll0`/`numcompare` esperam C-strings), e devolve o byte original depois — evita copiar a chave pra um buffer C-string separado.

**6. Macro segura com `do { ... } while (0)`** — `CMP_WITH_IGNORE` (`sort.c:3052-3074`): embrulha um bloco com `if`/`while` aninhados, permitindo chamar a macro como `CMP_WITH_IGNORE (a, b);` — com ponto e vírgula, em qualquer lugar que aceitaria uma instrução única — sem os bugs clássicos de macro multi-statement (dangling `else`, `if` sem chaves engolindo só o primeiro comando).

**7. `ATTRIBUTE_PURE`** — em `limfield` (`sort.c:1908`) e em duas funções vizinhas fora do subconjunto (`human_numcompare` `2218`, `numcompare` `2238`, pra contexto). É o atributo `pure` do GCC: diz ao compilador que a função não tem efeitos colaterais e o retorno depende só dos parâmetros, então chamadas repetidas podem ser eliminadas por CSE (common subexpression elimination) e participar de otimização de laço "do mesmo jeito que um operador aritmético" ([GCC, Common Function Attributes](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html)). Reduz instruções executadas via decisão do compilador, não do runtime — encaixa direto na definição do critério.

**8. Tabela de linhas bidirecional numa única alocação** — já detalhado em [Aritmética de ponteiros](#aritmética-de-ponteiros): texto crescendo pra frente e tabela de `struct line` crescendo pra trás dividem o mesmo bloco de memória. Evita duas alocações separadas (uma pro texto, outra pro array de structs) e mantém os dois contíguos — amigável a cache, sem indireção extra pra achar a linha N.

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
6. ~~Mapear ocorrências de aritmética de ponteiros~~ — feito, ver [Aritmética de ponteiros](#aritmética-de-ponteiros) acima.
7. ~~Levantar os "truques de programador C"~~ — feito, ver [Truques de programador C](#truques-de-programador-c) acima.
8. Dividir o subconjunto escolhido em blocos de responsabilidade e funções auxiliares, com tabela de dependências internas/externas (como a Tabela 1/2 do modelo).
9. Montar o diagrama estático (arquivo/funções e bibliotecas, como a Figura 1 do modelo) e o diagrama dinâmico (fluxo de execução, como a Figura 2).
10. Buscar ao menos uma referência acadêmica relacionada ao programa ou aos autores.
11. Preparar um exemplo de uso do programa (execução real, consumo de stack/heap) com depurador (gdb) ou ferramenta equivalente.
12. Criar um `Makefile` (ou script equivalente) para compilar/testar o trecho escolhido — cobre o critério de construção e testes automatizados.
13. Escrever o relatório seguindo a estrutura do modelo.
14. Preparar os slides/roteiro e gravar o vídeo (até 10 min), com cada integrante se identificando antes de falar.
15. Publicar o vídeo em plataforma de compartilhamento de mídia e entregar o link até 22/09.

## Working in this repo

When asked to help with this assignment, keep in mind the deliverables are a *written report* and a *video presenting it*, not a standalone program: work here will typically mean preparing source excerpts, annotated code walkthroughs, diagrams (static/dynamic UML), build/test scaffolding (e.g. a `Makefile` for the chosen `sort.c` subset), and supporting material — not building a new application from scratch. No coding is required by the assignment itself.
