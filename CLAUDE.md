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

Subconjunto escolhido para o relatório/apresentação (~677 linhas, fio narrativo único: buffer → extração de chave → comparação → dispatch; deixa de fora `main()`, 558 linhas, e a maquinaria de threads/merge, complexas demais pra um vídeo de 10 min):

| Função | Linhas | Motivo |
|---|---|---|
| `try_growbuf`/`maybe_growbuf` | 1802-1857 | truque: buffer dobra de tamanho (realloc amortizado) |
| `begfield` | 1862-1903 | aritmética de ponteiros — início do campo |
| `limfield` | 1908-2010 | aritmética de ponteiros — fim do campo, contador regressivo (inclui `ATTRIBUTE_PURE` em `1908`) |
| `fillbuf` | 2018-2133 | leitura de entrada/EOF, monta tabela de linhas via ponteiros |
| `keycompare` | 2946-3136 | núcleo: comparação multi-chave, bloco denso |
| `compare` | 3141-3181 | desempate por linha inteira |
| `sort()` | 4314-4441 | dispatcher memória vs. arquivo temporário; contém o único `goto` do arquivo fora de `check()` (`sort.c:4418`, label `finish`) |

> Correção (22/09): os limites de `begfield`, `limfield`, `fillbuf`, `compare` e `sort()` estavam contando até a linha anterior à *próxima* função, o que incluía comentário/atributo de documentação de outra função — não código da função em si. Corrigido pra fechar exatamente na chave de fechamento real de cada uma (total caiu de ~740 pra ~677 linhas; nenhuma citação `sort.c:linha` específica usada em Aritmética de ponteiros/Truques ficou fora do intervalo corrigido).

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

### Blocos de responsabilidade e dependências

Formato equivalente às Tabelas 1 (Dependências) e 2 (Blocos) do modelo — adaptado pra um subconjunto de 7 funções espalhadas pelo arquivo, em vez de um arquivo contínuo de 273 linhas como o `echo.c`.

#### Blocos internos de cada função

| Função | Sub-bloco | Linhas | Responsabilidade |
|---|---|---|---|
| `try_growbuf` | validação + alocação | `1805-1810` | confere se o novo tamanho é válido e maior que o atual; aloca o novo buffer |
| `try_growbuf` | cópia e realocação de ponteiros | `1812-1830` | copia texto e tabela de linhas pro novo buffer; corrige (*fixup*) os ponteiros internos de cada `struct line` pra apontarem pro novo endereço |
| `try_growbuf` | troca e limpeza | `1832-1836` | libera o buffer antigo, atualiza `buf->buf`/`buf->alloc` |
| `maybe_growbuf` | política de crescimento | `1843-1857` | decide se vale a pena crescer (respeitando limite), triplica o tamanho ou vai direto ao limite, chama `try_growbuf` e marca falha permanente se malloc negar |
| `begfield` | pula SWORD campos | `1872-1887` | dois caminhos: delimitador explícito (`-t`) via `memchr`, ou blanks (padrão POSIX) via varredura byte a byte |
| `begfield` | ajustes finais | `1891-1900` | pula blanks iniciais se `-b`; avança SCHAR bytes sem passar do limite |
| `limfield` | pula EWORD campos | `1925-1940` | espelha `begfield` pro fim do campo |
| `limfield` | código morto documentado | `1942-1991` | bloco `#ifdef POSIX_UNSPECIFIED` — nunca compilado por padrão; registra um bug de interpretação relatado por e-mail em 1996 (citado com autor e data no próprio comentário) que os mantenedores decidiram não resolver, por ambiguidade no texto do POSIX — bom material pra "Histórico"/curiosidade no vídeo |
| `limfield` | avança ECHAR bytes | `1993-2006` | mesmo idioma de `begfield`, pro fim do campo |
| `fillbuf` | reaproveita sobra do buffer | `2030-2035` | mistura pro início o que sobrou da leitura anterior (`buf->left`) |
| `fillbuf` | laço de leitura | `2037-2132` | laço externo `while (true)`: lê blocos de `fp`, localiza cada `'\n'` com `memchr`, preenche a tabela de linhas de trás pra frente, pré-computa a posição da primeira chave via `limfield`/`begfield` se houver `-k` |
| `fillbuf` | EOF e crescimento | `2058-2069`, `2124-2131` | trata fim de arquivo/erro de leitura; se uma linha não coube no buffer, aumenta via `maybe_growbuf`/`xpalloc` e tenta de novo |
| `keycompare` | prepara campo atual | `2949-2971` | usa posição de campo já calculada (1ª chave) ou recalcula (`limfield`/`begfield`) pras seguintes; mede o tamanho do campo |
| `keycompare` | despacho por tipo de chave | `2973-3043` | copia com tradução/ignore (pilha ou heap conforme tamanho) e despacha pro comparador certo: numérico, geral, humano, mês, aleatório, versão (`filenvercmp`) ou coleção locale-aware (`xmemcoll0`) |
| `keycompare` | comparação por ignore-set | `3050-3081` | usa a macro local `CMP_WITH_IGNORE` quando só há `-i`/`-d` sem tradução |
| `keycompare` | comparação simples | `3082-3105` | sem tradução nem ignore: `memcmp` direto (ou byte a byte se precisar traduzir sem ignorar) |
| `keycompare` | decide continuar ou parar | `3107-3133` | para na primeira chave com diferença; senão avança pra próxima `-k` |
| `compare` | tenta pelas chaves | `3150-3155` | delega pra `keycompare`; retorna cedo se houver diferença (ou se `-u`/`-s`) |
| `compare` | fallback pra linha inteira | `3159-3178` | sem chaves (ou empate nelas): compara a linha toda, locale-aware ou `memcmp` |
| `sort()` | prepara buffer/política | `4323-4358` | calcula `bytes_per_line` conforme número de threads; inicializa buffer só na primeira vez |
| `sort()` | laço de leitura via `fillbuf` | `4360-4419` | decide concatenar o próximo arquivo se couber no buffer; escolhe destino (saída final vs. temporário); despacha pro sort paralelo (`sortlines`) ou sequencial conforme `nthreads` |
| `sort()` | finalização (`goto finish`) | `4417-4441` | libera o buffer; se não escreveu direto na saída, junta (`merge`) os arquivos temporários; sempre espera os processos filhos de compressão (`reap_all`) |

#### Dependências internas (entre as 7 funções do subconjunto)

`sort()` → `fillbuf` → (`begfield`, `limfield`, `maybe_growbuf` → `try_growbuf`); `compare` → `keycompare` → (`begfield`, `limfield`). Ou seja: as duas funções de aritmética de ponteiros (`begfield`/`limfield`) são consumidas tanto na hora de ler o arquivo (`fillbuf`, pré-computo da 1ª chave) quanto na hora de comparar (`keycompare`, chaves seguintes).

#### Dependências externas (1º uso dentro do subconjunto)

| Origem | Identificador | Linha |
|---|---|---|
| `<string.h>`/`<stdlib.h>` | `malloc`, `free`, `memcpy`, `memchr`, `memmove`, `memcmp` | `1808`, `1832`, `1816`, `1875`, `2032`, `3101` |
| `<stdio.h>` | `fread`, `ferror`, `feof` | `2053`, `2060`, `2062` |
| gnulib/coreutils (`system.h` e módulos) | `to_uchar`, `ATTRIBUTE_PURE`, `MAX`, `MIN`, `NONZERO`, `_GL_CMP`, `xmalloc`, `xpalloc`, `xmemcoll0`, `filenvercmp` | `1883`, `1908`, `2083`, `3084`, `3038`, `3104`, `3000`, `2129`, `3042`, `3032` |
| `sort.c` — funções auxiliares fora do subconjunto | `buffer_linelim`, `line_aligned_size`, `sort_die`, `key_numeric`, `numcompare`, `general_numcompare`, `human_numcompare`, `getmonth`, `compare_random`, `diff_reversed` | `1813`, `1805`, `2061`, `2973`, `3022`, `3024`, `3026`, `3028`, `3030`, `3135` |
| `sort.c` — maquinaria de threads/merge (fora do subconjunto, só chamada por `sort()`) | `xfopen`, `sort_buffer_policy`, `initbuf`, `create_temp`, `queue_init`, `merge_tree_init`, `sortlines`, `sequential_sort`, `write_unique`, `merge_tree_destroy`, `queue_destroy`, `xfclose`, `xnmalloc`, `merge`, `reap_all` | `4331`, `4352`, `4354`, `4387`, `4394`, `4396`, `4398`, `4406`, `4409`, `4401`, `4402`, `4379`, `4429`, `4436`, `4440` |

A última linha da tabela é importante pro roteiro do vídeo: `sort()` é apresentado como *dispatcher* (o bloco "laço de leitura via `fillbuf`" acima), mas o interior de `sortlines`/`merge`/`queue_*` fica fora do subconjunto — vale dizer isso explicitamente na apresentação, não deixar implícito.

### Diagramas estático e dinâmico

Desenhados à mão em SVG (sem gerador automático), estilo simples preto e branco — retângulo/losango/seta, igual ao yUML usado no modelo do `echo.c`, pra ficar limpo tanto no relatório impresso quanto no vídeo.

**Estático** — [Trabalho1/diagramas/diagrama-estatico-sort.svg](Trabalho1/diagramas/diagrama-estatico-sort.svg): grafo de chamadas entre as 7 funções do subconjunto (não é um diagrama de arquivo/biblioteca como o do modelo, porque aqui as 7 funções estão espalhadas por um único arquivo de 5154 linhas — um grafo de chamadas conta uma história mais útil). Mostra as duas árvores de entrada (`sort()` e `compare()`, cada uma chamada por código fora do subconjunto, em caixa tracejada) convergindo em `begfield()`/`limfield()`, que são consumidas tanto na leitura (`fillbuf`, pré-computa a 1ª chave) quanto na comparação (`keycompare`, chaves seguintes) — o "X" no meio do diagrama *é* o achado central da seção de Aritmética de ponteiros, desenhado. Dependências externas (libc/gnulib/threads) ficam de fora do desenho (já estão na tabela acima) — a legenda completa vai na legenda da figura no relatório, não dentro do SVG.

**Dinâmico** — [Trabalho1/diagramas/diagrama-dinamico-sort.svg](Trabalho1/diagramas/diagrama-dinamico-sort.svg): diagrama de atividades UML do fluxo real de `sort()` durante uma execução — início/fim (bolinha preta / bolinha com anel), decisões em losango (`-k` definidas? mais de uma linha? `nthreads > 1`? há mais dados? gravou direto na saída?) e um laço de volta (rotulado "próximo bloco/arquivo") de `sort()` reconsumindo `fillbuf()` até esgotar a entrada. As duas caixas com borda mais grossa (`fillbuf()` e `sequential_sort()`→`compare()`→`keycompare()`) marcam onde o subconjunto estudado entra na execução; as caixas tracejadas (`sortlines()`, `merge()`) marcam o que fica de fora. Geometria conferida linha a linha contra o código-fonte antes de desenhar (não é um fluxo inventado) — ver [Blocos de responsabilidade](#blocos-de-responsabilidade-e-dependências) acima pra cada chamada.

Ambos verificados visualmente no navegador antes de finalizar (sem sobreposição de texto/linhas). No `.docx` final, exportar cada um como imagem (PNG em resolução alta) e numerar como Figura 1/Figura 2, com a legenda descritiva no corpo do relatório — não dentro do SVG.

### Referência acadêmica

O critério pede "ao menos uma"; o modelo do `echo.c` cita 3 (GNU 2021, Mayrhauser e Vans 1995, Spinellis 2003) — seguimos o mesmo padrão com 4, cada uma com um papel diferente (não é enchimento):

1. **Sobre o programa** — Donald E. Knuth, *The Art of Computer Programming, Volume 3: Sorting and Searching*, 2ª edição, Addison-Wesley, 1998, ISBN 978-0-201-89685-5, Seção 5.4 "External Sorting" (5.4.1 "Multiway Merging and Replacement Selection"). Referência canônica do algoritmo que `sort()` implementa: despachar entre ordenação em memória e merge sort externo via arquivos temporários. A maquinaria de merge que ficou fora do subconjunto (`merge()`, `queue_*`, `merge_tree_*`, ver [Blocos de responsabilidade](#blocos-de-responsabilidade-e-dependências)) é exatamente o "multiway merging" descrito por Knuth.
2. **Sobre o autor, sistemas** — Paul Eggert e Douglas Stott Parker Jr., "File Systems in User Space", USENIX Winter 1993. Tema de sistemas, mais próximo do escopo de "software básico" da disciplina do que os RFCs de fuso horário.
3. **Sobre o autor, padronização** — Paul Eggert et al., RFC 8536, "The Time Zone Information Format (TZif)", IETF, 2019. Trabalho de padronização real, não só manutenção de código.
4. **Sobre a metodologia** — Anneliese Mayrhauser e A. Marie Vans, "Program comprehension during software maintenance and evolution", *Computer* 28, set. 1995, pp. 44–55, doi: 10.1109/2.402076. Mesma referência citada pelo modelo do `echo.c`; embasa academicamente a técnica de dividir o código em blocos e funções auxiliares que usamos em [Blocos de responsabilidade](#blocos-de-responsabilidade-e-dependências).

### Exemplo de uso (execução, stack/heap)

Não deu pra usar gdb interativo: o gcc local (MSYS2, `C:\msys64\ucrt64\bin\gcc.exe`) está sendo bloqueado silenciosamente pelo ambiente — provavelmente o mesmo antivírus/EDR corporativo que também bloqueou o `pacman` (erros de certificado SSL nos espelhos MSYS2 ao tentar instalar o pacote do gdb). Não tentamos contornar segurança do sistema. A alternativa usada foi rodar o exemplo num compilador online (gcc 13.2.0 real, via API do [Wandbox](https://wandbox.org)) e usar a saída de execução real de lá — não uma previsão nossa.

[Trabalho1/exemplo-uso/harness_subconjunto.c](Trabalho1/exemplo-uso/harness_subconjunto.c) — cópia **verbatim** de 5 funções do subconjunto (`buffer_linelim`, `line_aligned_size`, `try_growbuf`, `maybe_growbuf`, `begfield`, `limfield`; cada uma com o comentário `/* sort.c:linha-linha */` apontando a origem), mais os stubs mínimos necessários (`struct line`/`struct keyfield`/`struct buffer` só com os campos usados, `to_uchar`, `blanks[]`, `ATTRIBUTE_PURE`) pra compilar isolado — o `sort.c` completo não compila sem a árvore de build inteira do coreutils/gnulib (config.h, system.h, argmatch.h etc.), que está fora do escopo do subconjunto.

[Trabalho1/exemplo-uso/saida-exemplo.txt](Trabalho1/exemplo-uso/saida-exemplo.txt) — saída real da execução (status 0, sem erros/warnings). Duas demonstrações:

1. **Heap** — `try_growbuf`/`maybe_growbuf` fazendo um buffer de 64 bytes crescer até passar de 5000: `64 → 224 → 704 → 2144 → 5024` bytes, com endereço de heap diferente a cada rodada (confirma que é `malloc` de um bloco novo + `free` do antigo, não `realloc` in-place) — números reais que batem com a conta feita à mão em [Truques de programador C](#truques-de-programador-c) (crescimento geométrico ×3, com o detalhe de `line_aligned_size` sempre arredondar pra cima em pelo menos um `sizeof(struct line)` extra).
2. **Stack** — `begfield`/`limfield` extraindo o campo 2 (`-k2,2`) da linha `"banana 42 2024-01-05"`, com endereços de pilha reais de `main()` e de um nível abaixo (`demo_extrair_campo`), mostrando o quadro de pilha sendo empilhado a cada chamada.

**Achado não planejado, só apareceu ao rodar de verdade:** o campo extraído veio `" 42"` — com o espaço na frente, não `"42"` limpo. Não é bug: é o comportamento documentado no próprio comentário de `begfield` (`sort.c:1869-1871`, "the leading field separator itself is included in a field when -t is absent"), confirmado na prática porque o harness roda com `skipsblanks=false` (equivalente a `sort -k2,2` sem `-b`). Prova que o código foi executado e entendido, não só lido — bom ponto pra puxar no vídeo.

### Construção e testes automatizados

**Atenção redobrada aqui:** a professora enfatizou em aula que testes automatizados costumam sair mal-feitos ("muita gente não sabe fazê-los"). O enunciado não fixa uma quantidade mínima de testes — **estipulamos a nossa**: pelo menos 1 teste de caminho normal + 1 de caso de borda para cada dupla de funções do subconjunto com lógica condicional não trivial. Resultado: **12 testes**, cobrindo as 6 funções do subconjunto (5 sobre `try_growbuf`/`maybe_growbuf`/heap, 5 sobre `begfield`/`limfield`/ponteiros — as duas restantes, `buffer_linelim`/`line_aligned_size`, são testadas indiretamente através das outras, por serem helpers pequenos demais pra merecer teste isolado).

- [Trabalho1/exemplo-uso/subconjunto.h](Trabalho1/exemplo-uso/subconjunto.h) — as 6 funções verbatim + stubs mínimos, extraído do harness do passo 11 pra ser incluído tanto pela demo quanto pelos testes (header-only, sem duplicar código).
- [Trabalho1/exemplo-uso/testes.c](Trabalho1/exemplo-uso/testes.c) — suíte de testes sem framework externo: um `check()` com contador de passa/falha, saída com `exit(1)` se qualquer teste falhar (pensado pra `make test`/CI, não pra conferência visual).
- [Trabalho1/exemplo-uso/Makefile](Trabalho1/exemplo-uso/Makefile) — alvos `all`/`run`/`test`/`clean`. Usamos um Makefile plano em vez do CMake do `mflash/DevCPP` porque o critério nomeia "make" especificamente e é mais fácil de ler/avaliar num relatório — o DevCPP (achado pelo usuário no Moodle da disciplina, não é obra acadêmica, por isso não entra nas [Referências acadêmicas](#referência-acadêmica)) serviu de referência de toolchain (gcc), não de sistema de build.
- [Trabalho1/exemplo-uso/saida-testes.txt](Trabalho1/exemplo-uso/saida-testes.txt) — saída real de `make test` (mesmo processo do passo 11: gdb/gcc local bloqueado pelo ambiente, rodamos num compilador online real via API do Wandbox). **12/12 passando**, status 0.

**Achado real ao rodar os testes pela primeira vez (não inventado pra parecer bonito):** na primeira rodada deu 11/12 — um teste sobre `try_growbuf` falhou porque a *expectativa do teste* estava errada, não o código de `sort.c`. A comparação `alloc <= buf->alloc` (`sort.c:1805`) usa o valor **já alinhado** por `line_aligned_size` (`sort.c:1623-1636`), não o valor original passado — e `line_aligned_size` sempre aumenta o valor em pelo menos 1 byte, mesmo quando já é múltiplo do alinhamento (o padding vira um bloco inteiro em vez de zero, porque `alignment - size % alignment` dá `alignment` quando o resto é 0 — o mesmo detalhe já registrado em [Truques de programador C](#truques-de-programador-c)). Corrigido o teste pra refletir esse comportamento real; histórico completo em `saida-testes.txt` e no commit `97d25ce`. Vale contar isso no vídeo — mostra que os testes são reais, não decoração.

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

Fonte normativa: [GNU Coding Standards, seção Formatting](https://www.gnu.org/prep/standards/html_node/Formatting.html) (citações diretas). Evidência levantada no subconjunto escolhido (linhas 1802-4441):

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
8. ~~Dividir o subconjunto em blocos de responsabilidade e dependências~~ — feito, ver [Blocos de responsabilidade e dependências](#blocos-de-responsabilidade-e-dependências) acima.
9. ~~Montar o diagrama estático e o diagrama dinâmico~~ — feito, ver [Diagramas estático e dinâmico](#diagramas-estático-e-dinâmico) acima.
10. ~~Buscar ao menos uma referência acadêmica~~ — feito, ver [Referência acadêmica](#referência-acadêmica) acima.
11. ~~Preparar um exemplo de uso~~ — feito, ver [Exemplo de uso](#exemplo-de-uso-execução-stackheap) acima.
12. ~~Criar Makefile e testes automatizados~~ — feito, ver [Construção e testes automatizados](#construção-e-testes-automatizados) acima.
13. Escrever o relatório seguindo a estrutura do modelo.
14. Preparar os slides/roteiro e gravar o vídeo (até 10 min), com cada integrante se identificando antes de falar.
15. Publicar o vídeo em plataforma de compartilhamento de mídia e entregar o link até 22/09.

## Working in this repo

When asked to help with this assignment, keep in mind the deliverables are a *written report* and a *video presenting it*, not a standalone program: work here will typically mean preparing source excerpts, annotated code walkthroughs, diagrams (static/dynamic UML), build/test scaffolding (e.g. a `Makefile` for the chosen `sort.c` subset), and supporting material — not building a new application from scratch. No coding is required by the assignment itself.
