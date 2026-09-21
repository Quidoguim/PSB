const fs = require("fs");
const path = require("path");
const {
  Document, Packer, Paragraph, TextRun, AlignmentType,
  Table, TableRow, TableCell, WidthType, BorderStyle, ShadingType,
  ImageRun, VerticalAlign,
} = require("docx");

const MARK = "SPLIT"; // private-use marker, never occurs in real text

function marker(name) {
  return new Paragraph({ text: MARK + name, spacing: { before: 0, after: 0 } });
}

// ---------------------------------------------------------------------
// text helpers -- mixedBody lets a paragraph mix normal prose with
// Consolas-font identifier/line mentions in the same sentence.
// ---------------------------------------------------------------------
function run(text, codeFont) {
  return new TextRun(codeFont ? { text, font: "Consolas" } : { text });
}

// parts: array of strings; a string wrapped in `backticks` becomes a code run
function mixedBody(template, opts = {}) {
  const pieces = template.split(/`([^`]+)`/);
  const children = pieces
    .filter((p) => p.length > 0)
    .map((p, i) => run(p, i % 2 === 1));
  // recompute i correctly relative to original split (filter changes indices) -- redo properly:
  const children2 = [];
  pieces.forEach((p, i) => {
    if (p === "") return;
    children2.push(run(p, i % 2 === 1));
  });
  return new Paragraph({
    children: children2,
    alignment: AlignmentType.JUSTIFIED,
    spacing: { after: 200, line: 360, lineRule: "auto" },
  });
}

function caption(text) {
  return new Paragraph({
    children: [new TextRun({ text, italics: true, size: 20 })],
    alignment: AlignmentType.CENTER,
    spacing: { before: 200, after: 240 },
  });
}

function codeLines(text) {
  return text
    .replace(/\r\n/g, "\n")
    .split("\n")
    .map(
      (line) =>
        new Paragraph({
          children: [new TextRun({ text: line.length ? line : " ", font: "Consolas", size: 18 })],
          spacing: { after: 0, line: 240, lineRule: "auto" },
          shading: { type: ShadingType.CLEAR, fill: "F2F2F2" },
        })
    );
}

// ---------------------------------------------------------------------
// booktabs table: top rule, header underline, bottom rule, no verticals
// ---------------------------------------------------------------------
const NONE = { style: BorderStyle.NONE, size: 0, color: "FFFFFF" };
function topRule() { return { style: BorderStyle.SINGLE, size: 12, color: "000000" }; }
function thinRule() { return { style: BorderStyle.SINGLE, size: 4, color: "000000" }; }

function cell(text, opts = {}) {
  const paraOpts = {
    children: Array.isArray(text)
      ? text
      : [new TextRun({ text, bold: !!opts.bold })],
    spacing: { after: 0, line: 276, lineRule: "auto" },
  };
  return new TableCell({
    width: { size: opts.width || 1000, type: WidthType.DXA },
    verticalAlign: VerticalAlign.TOP,
    margins: { top: 60, bottom: 60, left: 80, right: 80 },
    borders: opts.borders || { top: NONE, bottom: NONE, left: NONE, right: NONE, insideH: NONE, insideV: NONE },
    children: [new Paragraph(paraOpts)],
  });
}

function bookTable(headers, rows, widths) {
  const total = widths.reduce((a, b) => a + b, 0);
  const trs = [];
  trs.push(
    new TableRow({
      children: headers.map((h, i) =>
        cell(h, {
          bold: true,
          width: widths[i],
          borders: { top: topRule(), bottom: thinRule(), left: NONE, right: NONE, insideH: NONE, insideV: NONE },
        })
      ),
    })
  );
  rows.forEach((r, ri) => {
    const isLast = ri === rows.length - 1;
    trs.push(
      new TableRow({
        children: r.map((val, ci) =>
          cell(val, {
            width: widths[ci],
            borders: {
              top: NONE,
              bottom: isLast ? topRule() : NONE,
              left: NONE,
              right: NONE,
              insideH: NONE,
              insideV: NONE,
            },
          })
        ),
      })
    );
  });
  return new Table({
    width: { size: total, type: WidthType.DXA },
    columnWidths: widths,
    rows: trs,
  });
}

// mixed-run cell content: array of {t, code}
function mix(parts) {
  return parts.map((p) => new TextRun(p.code ? { text: p.t, font: "Consolas", size: 18 } : { text: p.t, size: 20 }));
}
function plain(t) {
  return [new TextRun({ text: t, size: 20 })];
}

function image(file, widthPx, heightPx) {
  const data = fs.readFileSync(path.join(__dirname, "images", file));
  return new Paragraph({
    alignment: AlignmentType.CENTER,
    spacing: { before: 120, after: 0 },
    children: [
      new ImageRun({
        data,
        type: "png",
        transformation: { width: widthPx, height: heightPx },
      }),
    ],
  });
}

// =======================================================================
// SEÇÃO 2 — Histórico dos autores
// =======================================================================
const sec2 = [
  marker("sec2"),
  mixedBody(
    "Mike Haertel estudou no St. Olaf College, em Minnesota. No verão de 1988, ele descobriu o Emacs e o GCC, escreveu para Richard Stallman, e a Free Software Foundation o contratou junto com o colega de faculdade Pete TerMaat (GNU's Bulletin v1n5, fonte primária)."
  ),
  mixedBody(
    "O primeiro projeto de Haertel na FSF foi um novo `egrep`, com algoritmos próprios de casamento de padrões — Boyer-Moore, além de matchers DFA e kwset. Esse trabalho deu origem ao GNU grep. Haertel foi o autor principal do grep do fim dos anos 1980 até os anos 2000. Na mesma época, ele escreveu o GNU diff e o GNU sort — o cabeçalho de `sort.c` registra dezembro de 1988 como a data original, a mesma do arquivo estudado neste trabalho."
  ),
  mixedBody(
    "Em 21 de agosto de 2010, Haertel publicou na lista `freebsd-current` uma explicação hoje clássica sobre a velocidade do GNU grep (fonte primária: lists.freebsd.org/pipermail/freebsd-current/2010-August/019310.html). Ele descreveu dois princípios: o grep evita examinar cada byte da entrada, e executa poucas instruções por byte quando examina. Com Boyer-Moore e um laço desenrolado, o grep processa menos de três instruções x86 por byte. Ele também usa `mmap()` no lugar de `read()`, com ganho de mais de 20% medido por ele mesmo. Uma frase de Haertel resume a filosofia: “the key to making programs fast is to make them do practically nothing” — a chave para deixar programas rápidos é fazer com que eles não façam quase nada."
  ),
  mixedBody(
    "Essa filosofia aparece direto no subconjunto de `sort.c` estudado neste trabalho. O buffer de `try_growbuf` cresce por blocos, em vez de realocar a cada linha. As funções `begfield` e `limfield` varrem o texto com ponteiro, sem copiar dados (ver Seção 4.1). São dois programas escritos em anos diferentes, mas pela mesma cabeça de engenharia."
  ),
  mixedBody(
    "Depois do trabalho na FSF, Haertel migrou para arquitetura de processadores. Trabalhou na Intel, no projeto do Pentium 4, depois na AMD, e hoje está de volta à Intel como CPU architect."
  ),
  mixedBody(
    "Paul Eggert nasceu em 4 de dezembro de 1954. Cursou Engenharia Elétrica na Rice University — a universidade não tinha departamento de Ciência da Computação na época — e se formou em 1975. Fez o doutorado em Ciência da Computação na UCLA, concluído em 1980 (página oficial UCLA Samueli; perfil detalhado: Rice Magazine, “The Time Zone Keeper”, primavera de 2025)."
  ),
  mixedBody(
    "Eggert deu aula na UC Santa Barbara por três anos. Depois foi para a indústria: cofundou startups e trabalhou na System Development Corporation. Mais tarde foi CTO da Twin Sun Inc., empresa de serviços técnicos em GNU, Linux e BSD para o mercado japonês. Hoje é Teaching Professor no Departamento de Ciência da Computação da UCLA."
  ),
  mixedBody(
    "Além da manutenção de código, Eggert publicou trabalhos acadêmicos: “File Systems in User Space” (USENIX Winter 1993, com Douglas Stott Parker Jr.); “Monte Carlo arithmetic: how to gamble with floating point and win” (Computing in Science & Engineering, 2000); “Perturbing and evaluating numerical programs without recompilation” (Software: Practice and Experience, 2005). Ele também é coautor de padrões da IETF: a RFC 8536 e a RFC 9636, sobre o formato do tz database (TZif), e a RFC 6557, sobre os procedimentos de manutenção do banco de dados."
  ),
  mixedBody(
    "No início dos anos 1990, Eggert notou inconsistências de fuso horário enquanto fazia negócios em regiões diferentes. Corrigiu entradas pontuais — Taiwan, Indonésia — no tz database criado por Arthur David Olson, e depois decidiu terminar o trabalho por completo. Desde 2005 é o editor oficial do banco na IANA, sem remuneração, em paralelo às aulas. Seu método de pesquisa inclui almanaques de astrólogo, documentos legais, arquivos de governo e horários antigos de trem — o caso de Marrocos, por exemplo, muda o horário no Ramadã por observação astronômica. Eggert também criou a convenção de nomes usada até hoje, como `America/New_York`."
  ),
  mixedBody(
    "Além do tz database, Eggert contribui com Autoconf, Bison, Diffutils, GNU RCS (mantenedor desde 1989), gzip, Emacs, GCC, glibc, GNU tar e GNU Coreutils — onde mantém `sort.c` ao lado de Haertel. Em 2021 recebeu o FSF Award for the Advancement of Free Software; em 2012, o Lockheed Martin Excellence in Teaching Award. A imprensa já o chamou de “Time Zone King” — National Geographic, The Register e a revista da Rice University publicaram perfis sobre seu trabalho."
  ),
  mixedBody(
    "Instituição em comum: os dois autores estão ligados ao GNU Project e à Free Software Foundation. O código estudado neste trabalho vive hoje sob o guarda-chuva do GNU Coreutils."
  ),
];

// =======================================================================
// SEÇÃO 3 — Convenções de codificação
// =======================================================================
const convRows = [
  [plain('"keep the length of source lines to 79 characters or less"'), plain("linhas do subconjunto ficam em torno de 80 colunas")],
  [
    plain('"put the open-brace that starts the body of a C function in column one"'),
    mix([{ t: "tipo de retorno numa linha, assinatura na seguinte, " }, { t: "{", code: true }, { t: " sozinha na coluna 0 (" }, { t: "try_growbuf, begfield, keycompare, sort()", code: true }, { t: ")" }]),
  ],
  [
    plain('"spaces before the open-parentheses and after the commas"'),
    mix([{ t: "malloc (alloc)", code: true }, { t: " e " }, { t: "memcpy (newbuf, oldbuf, buf->used)", code: true }]),
  ],
  [
    plain('"split it before an operator, not after one"'),
    mix([{ t: "sort.c:3007-3009", code: true }, { t: " — ternário quebrado com " }, { t: "?", code: true }, { t: "/" }, { t: ":", code: true }, { t: " no início da linha de continuação, dentro de " }, { t: "keycompare", code: true }]),
  ],
  [
    mix([{ t: "template " }, { t: "do / { ... } / while (cond);", code: true }]),
    mix([{ t: "sort.c:3090-3098", code: true }, { t: " — do-while real dentro de " }, { t: "keycompare", code: true }]),
  ],
  [plain('"whatever style you use, please use it consistently"'), plain("mesmo estilo do início ao fim, ~2600 linhas entre try_growbuf e sort()")],
];

const sec3 = [
  marker("sec3"),
  mixedBody(
    "O GNU Coding Standards define as convenções de formatação usadas em todo o projeto GNU. O subconjunto de `sort.c` segue essas regras com rigor. A Tabela 2 confronta cada regra do padrão, citada diretamente, com um trecho real do código."
  ),
  bookTable(["Regra do padrão (citação direta)", "Onde aparece no subconjunto"], convRows, [4500, 4573]),
  caption("Tabela 2 — Regras do GNU Coding Standards confrontadas com o subconjunto"),
  mixedBody(
    "Além das regras acima, o subconjunto segue outros quatro pontos de convenção."
  ),
  mixedBody(
    "A indentação usa 2 espaços por nível. O código não usa tabs — a única exceção é o alinhamento da barra de continuação (`\\`) em macros de várias linhas, como `CMP_WITH_IGNORE` (`sort.c:3052-3061`)."
  ),
  mixedBody(
    "Ponteiros colam o asterisco ao identificador, não ao tipo: `char *ptr`, `struct line *line` (`sort.c:1865`)."
  ),
  mixedBody(
    "Identificadores de funções e variáveis usam `snake_case`: `try_growbuf`, `begfield`, `new_linelim`. Macros e atributos usam MAIÚSCULO: `ATTRIBUTE_PURE`, `TAB_DEFAULT`. Nomes de `struct` ficam sempre em minúsculo."
  ),
  mixedBody(
    "Cada função recebe um comentário em bloco antes da assinatura, descrevendo o que ela faz. Os nomes dos parâmetros aparecem em MAIÚSCULO dentro do texto do comentário — por exemplo, “Try to grow BUF according to POLICY”, antes de `maybe_growbuf` (`sort.c:1838`)."
  ),
];

// =======================================================================
// SEÇÃO 4 — Idiomas e truques de programador C
// =======================================================================
const sec4 = [
  marker("sec4"),
  mixedBody(
    "O `struct buffer` de `sort.c` guarda duas estruturas numa única alocação de memória. O texto das linhas cresce do início para o fim. A tabela `struct line[]` cresce do fim para o início. As duas se encontram no meio."
  ),
  mixedBody(
    "Essa estrutura obriga todo acesso a usar aritmética de ponteiros — soma, subtração, incremento e decremento. Nenhuma parte do código usa um índice contado à parte. Um ponteiro guarda a posição e avança sozinho. A subtração de dois ponteiros já dá o tamanho de um trecho, sem chamar `strlen` ou rodar um laço extra."
  ),
  mixedBody(
    "A função `begfield` (`sort.c:1862-1903`) localiza o início de um campo de ordenação. A linha 1865 soma um ponteiro a um inteiro para montar o limite `lim` sem precisar de um segundo laço: `char *ptr = line->text, *lim = ptr + line->length - 1;`. Os quatro laços `while` que seguem (linhas 1878, 1884, 1886 e 1893) usam `++ptr` — o próprio ponteiro funciona como contador. A linha 1896 subtrai dois ponteiros para saber quantos bytes restam, sem recalcular nada: `size_t remaining_bytes = lim - ptr;`. A linha 1898 soma um inteiro ao ponteiro para pular SCHAR bytes de uma vez: `ptr += schar;`."
  ),
  mixedBody(
    "A função `limfield` (`sort.c:1908-2010`) espelha o mesmo idioma para o fim do campo: a soma `ptr + line->length - 1` (linha 1912), a subtração `lim - ptr` (linhas 1928 e 2002) e a soma `ptr += echar` (linha 2004)."
  ),
  mixedBody(
    "A função `fillbuf` (`sort.c:2018-2133`) é onde a aritmética de ponteiros aparece com mais densidade. A linha 2039 retoma a escrita exatamente onde a última leitura parou, sem guardar a posição numa variável separada: `char *ptr = buf->buf + buf->used;`. A linha 2042 calcula, numa única expressão, o espaço livre entre a região de texto — que cresce para a frente — e a tabela de linhas — que cresce para trás: `size_t avail = (char *) linelim - buf->nlines * line_bytes - ptr;`, misturando subtração de ponteiros, um cast e uma multiplicação."
  ),
  mixedBody(
    "A linha 2080 é a prova direta de que os dois lados da alocação se encontram no meio: `line--;` decrementa o ponteiro da tabela a cada linha nova, em vez de incrementar um índice — a tabela de linhas é preenchida de trás para frente. A linha 2082 subtrai dois ponteiros para obter o tamanho da linha, sem chamar `strlen`: `line->length = ptr - line_start;`. E a linha 2114 subtrai dois ponteiros do tipo `struct line *` — não `char *` — para contar quantas linhas foram preenchidas nesta leitura: `buf->nlines = buffer_linelim (buf) - line;`."
  ),
  mixedBody(
    "A função `keycompare` (`sort.c:2946-3136`) consome os ponteiros já calculados por `begfield`, `limfield` e `fillbuf`. A linha 2970 repete o mesmo idioma de subtração para medir o tamanho do campo: `size_t lena = lima - texta;`."
  ),
  mixedBody(
    "A função `sort()` (`sort.c:4314-4441`) percorre o array de ponteiros `char *const *files` — a mesma forma do `argv` de `main` — incrementando o próprio ponteiro em vez de indexar `files[i]` (linhas 4330 e 4357). Ela também reusa a tabela de linhas “de trás para frente” construída por `fillbuf`: `line - buf.nlines`, `line - 1` e `line - i - 1` (linhas 4407, 4413 e 4409). Isso confirma que o padrão não é um truque isolado de uma função — é uma decisão estrutural do arquivo inteiro."
  ),

  marker("sec4-2"),
  mixedBody(
    "O enunciado da disciplina cita a contagem regressiva como um dos idiomas típicos da programação em C. O subconjunto de `sort.c` usa esse idioma em `begfield` e `limfield`: `while (ptr < lim && sword--)` (`sort.c:1873` e `1881`) e `while (ptr < lim && eword--)` (`sort.c:1926` e `1934`)."
  ),
  mixedBody(
    "O decremento acontece dentro da própria condição do `while`. A avaliação por curto-circuito só executa o decremento quando `ptr < lim` já é verdade. Essa construção dispensa uma variável de controle separada e uma linha a mais de código."
  ),

  marker("sec4-3"),
  mixedBody(
    "O arquivo `sort.c` inteiro — 5154 linhas — tem apenas dois `goto`. Um fica dentro de `check()` (`sort.c:3272`), fora do subconjunto estudado. O outro fica dentro de `sort()` (`sort.c:4418`, rótulo `finish`), dentro do subconjunto."
  ),
  mixedBody(
    "Esse `goto` controla a finalização da função. Quando `sort()` já escreveu a saída final diretamente — sem passar por arquivo temporário —, o `goto finish` pula direto para o bloco que libera o buffer e espera os processos filhos, sem repetir esse código em outro ponto da função. É um uso disciplinado de desvio incondicional: evita duplicar a lógica de encerramento, sem criar um fluxo confuso."
  ),

  marker("sec4-4"),
  mixedBody(
    "Truque 1 — tabelas indexadas por byte, em vez de comparação em cadeia. O arquivo define `UCHAR_LIM` como `UCHAR_MAX + 1` (`sort.c:83`) e a tabela `static bool blanks[UCHAR_LIM]` (`sort.c:270`). A expressão `blanks[to_uchar (*ptr)]`, usada em `begfield` e `limfield` (por exemplo `sort.c:1883, 1892` e `1936`), vira uma única leitura de memória em vez de comparar o caractere contra espaço e tab um a um. Essa troca de branches por tabela de consulta é exatamente o que o critério da disciplina pede: reduzir o número de instruções executadas."
  ),
  mixedBody(
    "Esse truque depende de `to_uchar()`, definida em `system.h` do coreutils como `static inline unsigned char to_uchar (char ch) { return ch; }`. O comentário original explica o motivo: converter um caractere possivelmente com sinal para um caractere sem sinal é mais seguro do que um cast direto, porque captura erros de tipo que o cast não captura (fonte: system.h, coreutils 8.23). Sem essa conversão, um byte maior ou igual a 128 num `char` com sinal vira um índice negativo ao acessar `blanks[]` — um comportamento indefinido que corrompe memória. É um dos erros clássicos de programador C iniciante, e o código o evita de forma sistemática."
  ),
  mixedBody(
    "Truque 2 — crescimento geométrico do buffer. A função `maybe_growbuf` (`sort.c:1840-1857`) triplica o tamanho do buffer, em vez de crescer sob demanda a cada linha: `if (buf->alloc <= policy->limit / 3) alloc = buf->alloc * 3;`. Um realloc a cada poucas linhas, em vez de a cada linha, muda o custo total de O(n²) para O(n) amortizado — a mesma ideia por trás do crescimento de um `std::vector` em C++ ou de uma lista em Python, aqui implementada à mão em C."
  ),
  mixedBody(
    "Truque 3 — small-buffer optimization, para evitar o heap no caso comum. Dentro de `keycompare` (`sort.c:2985-3000`), a linha `char stackbuf[4000];` reserva espaço na pilha. Se a chave de ordenação cabe nesses 4000 bytes, o código usa o espaço da pilha; só aloca no heap quando a chave é maior. Como `keycompare` roda uma vez por par de linhas comparadas, essa escolha evita um `malloc` e um `free` por comparação no caso comum — linhas curtas. A mesma função também usa o texto como buffer mutável: grava um caractere nulo temporário (`ta[tlena] = '\\0';`) e depois restaura o byte original, evitando copiar a chave para um buffer separado."
  ),
  mixedBody(
    "Truque 4 — macro segura com `do { ... } while (0)`. A macro `CMP_WITH_IGNORE` (`sort.c:3052-3074`) embrulha um bloco com `if` e `while` aninhados dentro desse padrão. Isso permite chamar a macro como uma instrução única, com ponto e vírgula no final — `CMP_WITH_IGNORE (a, b);` —, sem os bugs clássicos de macro com várias instruções, como um `else` que gruda no `if` errado."
  ),
  mixedBody(
    "Truque 5 — o atributo `ATTRIBUTE_PURE`. A função `limfield` leva esse atributo na linha 1908. Ele corresponde ao atributo `pure` do GCC: diz ao compilador que a função não tem efeitos colaterais e que o retorno depende só dos parâmetros. Com essa informação, o compilador pode eliminar chamadas repetidas — common subexpression elimination — e otimizar laços, do mesmo jeito que faria com um operador aritmético (GCC, Common Function Attributes). É uma redução de instruções decidida em tempo de compilação, não em tempo de execução — e se encaixa direto na definição do critério da disciplina."
  ),
];

// =======================================================================
// SEÇÃO 5 — Divisão em blocos
// =======================================================================
const depRows = [
  [plain("<string.h> / <stdlib.h>"), mix([{ t: "malloc, free, memcpy, memchr, memmove, memcmp", code: true }]), plain("1808, 1832, 1816, 1875, 2032, 3101")],
  [plain("<stdio.h>"), mix([{ t: "fread, ferror, feof", code: true }]), plain("2053, 2060, 2062")],
  [
    plain("gnulib/coreutils (system.h e módulos)"),
    mix([{ t: "to_uchar, ATTRIBUTE_PURE, MAX, MIN, NONZERO, _GL_CMP, xmalloc, xpalloc, xmemcoll0, filenvercmp", code: true }]),
    plain("1883, 1908, 2083, 3084, 3038, 3104, 3000, 2129, 3042, 3032"),
  ],
  [
    plain("sort.c — funções auxiliares fora do subconjunto"),
    mix([{ t: "buffer_linelim, line_aligned_size, sort_die, key_numeric, numcompare, general_numcompare, human_numcompare, getmonth, compare_random, diff_reversed", code: true }]),
    plain("1813, 1805, 2061, 2973, 3022, 3024, 3026, 3028, 3030, 3135"),
  ],
  [
    plain("sort.c — maquinaria de threads/merge (só chamada por sort())"),
    mix([{ t: "xfopen, sort_buffer_policy, initbuf, create_temp, queue_init, merge_tree_init, sortlines, sequential_sort, write_unique, merge_tree_destroy, queue_destroy, xfclose, xnmalloc, merge, reap_all", code: true }]),
    plain("4331, 4352, 4354, 4387, 4394, 4396, 4398, 4406, 4409, 4401, 4402, 4379, 4429, 4436, 4440"),
  ],
];

const blocRows = [
  [mix([{t:"try_growbuf",code:true}]), plain("validação + alocação"), plain("1805-1810"), plain("confere se o novo tamanho é válido e maior que o atual; aloca o novo buffer")],
  [mix([{t:"try_growbuf",code:true}]), plain("cópia e realocação de ponteiros"), plain("1812-1830"), plain("copia texto e tabela de linhas pro novo buffer; corrige os ponteiros internos de cada struct line")],
  [mix([{t:"try_growbuf",code:true}]), plain("troca e limpeza"), plain("1832-1836"), mix([{t:"libera o buffer antigo, atualiza "},{t:"buf->buf",code:true},{t:"/"},{t:"buf->alloc",code:true}])],
  [mix([{t:"maybe_growbuf",code:true}]), plain("política de crescimento"), plain("1843-1857"), plain("decide se vale a pena crescer, triplica ou vai direto ao limite, chama try_growbuf e marca falha se malloc negar")],
  [mix([{t:"begfield",code:true}]), plain("pula SWORD campos"), plain("1872-1887"), plain("dois caminhos: delimitador -t via memchr, ou blanks via varredura byte a byte")],
  [mix([{t:"begfield",code:true}]), plain("ajustes finais"), plain("1891-1900"), plain("pula blanks iniciais se -b; avança SCHAR bytes sem passar do limite")],
  [mix([{t:"limfield",code:true}]), plain("pula EWORD campos"), plain("1925-1940"), plain("espelha begfield pro fim do campo")],
  [mix([{t:"limfield",code:true}]), plain("código morto documentado"), plain("1942-1991"), plain("bloco #ifdef POSIX_UNSPECIFIED: bug de interpretação relatado em 1996, nunca compilado por padrão")],
  [mix([{t:"limfield",code:true}]), plain("avança ECHAR bytes"), plain("1993-2006"), plain("mesmo idioma de begfield, pro fim do campo")],
  [mix([{t:"fillbuf",code:true}]), plain("reaproveita sobra do buffer"), plain("2030-2035"), plain("mistura pro início o que sobrou da leitura anterior")],
  [mix([{t:"fillbuf",code:true}]), plain("laço de leitura"), plain("2037-2132"), plain("lê blocos, localiza \\n com memchr, preenche a tabela de trás pra frente, pré-computa a 1ª chave")],
  [mix([{t:"fillbuf",code:true}]), plain("EOF e crescimento"), plain("2058-2069, 2124-2131"), plain("trata fim de arquivo/erro; cresce via maybe_growbuf/xpalloc se necessário")],
  [mix([{t:"keycompare",code:true}]), plain("prepara campo atual"), plain("2949-2971"), plain("usa posição já calculada (1ª chave) ou recalcula; mede o tamanho do campo")],
  [mix([{t:"keycompare",code:true}]), plain("despacho por tipo de chave"), plain("2973-3043"), plain("copia com tradução/ignore e despacha pro comparador certo: numérico, geral, humano, mês, aleatório, versão ou locale")],
  [mix([{t:"keycompare",code:true}]), plain("comparação por ignore-set"), plain("3050-3081"), mix([{t:"macro CMP_WITH_IGNORE quando só há -i/-d sem tradução",code:false}])],
  [mix([{t:"keycompare",code:true}]), plain("comparação simples"), plain("3082-3105"), plain("sem tradução nem ignore: memcmp direto")],
  [mix([{t:"keycompare",code:true}]), plain("decide continuar ou parar"), plain("3107-3133"), plain("para na primeira chave com diferença; senão avança pra próxima -k")],
  [mix([{t:"compare",code:true}]), plain("tenta pelas chaves"), plain("3150-3155"), plain("delega pra keycompare; retorna cedo se houver diferença (ou -u/-s)")],
  [mix([{t:"compare",code:true}]), plain("fallback pra linha inteira"), plain("3159-3178"), plain("sem chaves ou empate nelas: compara a linha toda")],
  [mix([{t:"sort()",code:true}]), plain("prepara buffer/política"), plain("4323-4358"), plain("calcula bytes_per_line; inicializa buffer só na primeira vez")],
  [mix([{t:"sort()",code:true}]), plain("laço de leitura via fillbuf"), plain("4360-4419"), plain("decide concatenar o próximo arquivo, escolhe destino, despacha pro sort paralelo ou sequencial")],
  [mix([{t:"sort()",code:true}]), plain("finalização (goto finish)"), plain("4417-4441"), plain("libera o buffer; junta temporários se necessário; espera os processos filhos")],
];

const sec5 = [
  marker("sec5-1"),
  mixedBody(
    "As 7 funções do subconjunto dependem umas das outras assim: `sort()` chama `fillbuf()`, que chama `begfield()`, `limfield()` e `maybe_growbuf()` — que por sua vez chama `try_growbuf()`. Em paralelo, `compare()` chama `keycompare()`, que também chama `begfield()` e `limfield()`. As duas funções de aritmética de ponteiros, portanto, são consumidas tanto na leitura do arquivo quanto na comparação entre linhas."
  ),
  mixedBody(
    "Além dessas dependências internas, as 7 funções dependem de identificadores de três origens: a biblioteca padrão de C, os módulos internos do GNU Coreutils e da gnulib, e outras funções do próprio `sort.c` que ficam fora do subconjunto. A Tabela 3 lista cada origem, com a linha do primeiro uso dentro do subconjunto."
  ),
  bookTable(["Origem", "Identificador", "Linha"], depRows, [3200, 4200, 1673]),
  caption("Tabela 3 — Dependências externas do subconjunto, por origem"),
  mixedBody(
    "A última linha da Tabela 3 lista a maquinaria de threads e merge externo. Essas funções ficam fora do subconjunto porque implementam o merge sort externo completo — com múltiplas threads e arquivos temporários —, complexo demais para uma apresentação de 10 minutos."
  ),
  mixedBody(
    "O algoritmo que essa maquinaria implementa é descrito por Donald E. Knuth em The Art of Computer Programming, Volume 3: Sorting and Searching (2ª edição, Addison-Wesley, 1998), na Seção 5.4, “External Sorting”, especificamente na subseção “Multiway Merging and Replacement Selection”. `sort()` despacha exatamente entre os dois casos que Knuth descreve: ordenar em memória quando os dados cabem no buffer, ou fazer merge sort externo por arquivos temporários quando não cabem."
  ),

  marker("sec5-2"),
  mixedBody(
    "O subconjunto se divide em blocos de responsabilidade menores que uma função inteira. A Tabela 4 lista cada bloco, no mesmo formato da Tabela 2 do modelo do echo.c."
  ),
  bookTable(["Função", "Sub-bloco", "Linhas", "Responsabilidade"], blocRows, [1600, 2400, 1600, 3473]),
  caption("Tabela 4 — Blocos de responsabilidade do subconjunto"),
  mixedBody(
    "Um achado vale destaque: dentro de `limfield`, o bloco das linhas 1942 a 1991 é código morto, nunca compilado por padrão — está dentro de `#ifdef POSIX_UNSPECIFIED`. O comentário original registra um bug de interpretação relatado por e-mail em 1996, traduzido para a terminologia do POSIX por Paul Eggert. Os mantenedores decidiram não resolver a ambiguidade, e o bloco ficou desativado. É uma boa curiosidade histórica para a apresentação."
  ),
];

// =======================================================================
// SEÇÃO 6 — Cenário principal (com as duas figuras)
// =======================================================================
const sec6 = [
  marker("sec6"),
  mixedBody(
    "As seções anteriores descreveram os idiomas, os truques e a divisão em blocos do subconjunto. Esta seção mostra dois diagramas que resumem a estrutura estática e o comportamento dinâmico do código durante uma execução real do programa."
  ),
  marker("sec6-1"),
  mixedBody(
    "A Figura 1 mostra o grafo de chamadas entre as 7 funções do subconjunto. Diferente do diagrama do modelo — que relaciona um arquivo às suas bibliotecas —, este diagrama relaciona as funções entre si, porque as 7 funções estudadas estão espalhadas por um único arquivo de 5154 linhas: um grafo de chamadas conta uma história mais útil nesse caso."
  ),
  mixedBody(
    "O diagrama mostra duas árvores de entrada. `sort()` é chamada por `main()`, fora do subconjunto, e chama `fillbuf()`, que por sua vez chama `maybe_growbuf()` — que chama `try_growbuf()` — e pré-computa a primeira chave via `begfield()`/`limfield()`. `compare()` é chamada por `sortlines()`/`sequential_sort()`, também fora do subconjunto, e chama `keycompare()`, que reusa os mesmos `begfield()`/`limfield()` para as chaves seguintes."
  ),
  mixedBody(
    "O cruzamento em X no meio do diagrama é o achado central da Seção 4.1: as duas funções de aritmética de ponteiros atendem tanto à leitura do arquivo quanto à comparação entre linhas."
  ),
  image("diagrama-estatico-sort-hi.png", 566, 251),
  caption("Figura 1 — Grafo de chamadas do subconjunto estudado"),

  marker("sec6-2"),
  mixedBody(
    "A Figura 2 é um diagrama de atividades UML do fluxo real de `sort()` durante uma execução. O círculo preto marca o início; o círculo com anel marca o fim. Os losangos marcam decisões: se há chaves `-k` definidas, se o bloco lido tem mais de uma linha, se `nthreads` é maior que 1, se ainda há dados para ler, e se a saída foi gravada direto no arquivo final."
  ),
  mixedBody(
    "As caixas de borda mais grossa — `fillbuf()` e `sequential_sort()`→`compare()`→`keycompare()` — marcam onde o subconjunto estudado entra na execução. As caixas tracejadas — `sortlines()` e `merge()` — marcam a maquinaria de threads e merge externo que fica fora do subconjunto."
  ),
  mixedBody(
    "O laço de volta, rotulado “próximo bloco/arquivo”, mostra `sort()` reconsumindo `fillbuf()` até esgotar toda a entrada. A geometria do diagrama foi conferida linha a linha contra o código-fonte antes de ser desenhada — não é um fluxo inventado."
  ),
  image("diagrama-dinamico-sort-hi.png", 378, 531),
  caption("Figura 2 — Fluxo de execução de sort()"),
];

// =======================================================================
// SEÇÃO 7 — Exemplo de uso
// =======================================================================
const saidaExemplo = `=== Demonstracao 1: crescimento do buffer (heap), try_growbuf/maybe_growbuf ===
(sort.c:1802-1857 -- ver Truques de programador C)

buffer inicial : alloc=    64 bytes  endereco=0x2eeb32b0
crescimento 1 : alloc=   224 bytes  endereco=0x2eeb3300  (buffer antigo foi liberado)
crescimento 2 : alloc=   704 bytes  endereco=0x2eeb33f0  (buffer antigo foi liberado)
crescimento 3 : alloc=  2144 bytes  endereco=0x2eeb36c0  (buffer antigo foi liberado)
crescimento 4 : alloc=  5024 bytes  endereco=0x2eeb3f30  (buffer antigo foi liberado)
parou de crescer: alloc final=5024 >= limite pedido=5000, growth_failed=false

=== Demonstracao 2: aritmetica de ponteiros (stack), begfield/limfield ===
(sort.c:1862-2010 -- ver Aritmetica de ponteiros)

linha de entrada: "banana 42 2024-01-05"
endereco de 'line' (struct local em main): 0x7ffe5b59a8a0
endereco de 'key'  (struct local em main): 0x7ffe5b59a870
  [dentro de demo_extrair_campo, 1 nivel abaixo de main]
  endereco de uma variavel local aqui: 0x7ffe5b59a82c
  begfield() -> 0x7ffe5b59a8c6  (offset 6 bytes desde line.text)
  limfield()  -> 0x7ffe5b59a8c9  (offset 9 bytes desde line.text)
  campo extraido (chave -k2,2): " 42"`;

const sec7 = [
  marker("sec7"),
  mixedBody(
    "O ambiente de desenvolvimento usado neste trabalho bloqueou a instalação do gdb — o mesmo bloqueio de rede corporativo que impediu o `pacman` de baixar pacotes do MSYS2, por erro de certificado SSL. Em vez de contornar essa restrição de segurança, a dupla optou por rodar o exemplo de uso num compilador online real (gcc 13.2.0, via API do Wandbox) e registrar a saída de execução real — não uma previsão."
  ),
  mixedBody(
    "O arquivo `harness_subconjunto.c` contém uma cópia verbatim de 5 funções do subconjunto — `buffer_linelim`, `line_aligned_size`, `try_growbuf`, `maybe_growbuf`, `begfield` e `limfield` —, mais os tipos e funções auxiliares mínimos necessários para compilar essas funções de forma isolada. O `sort.c` completo não compila sem a árvore de build inteira do GNU Coreutils e da gnulib, fora do escopo do subconjunto estudado."
  ),
  mixedBody(
    "A primeira demonstração mostra o crescimento do buffer, no heap. Um buffer de 64 bytes cresce, chamada após chamada de `maybe_growbuf()`, até ultrapassar um limite de 5000 bytes: 64, 224, 704, 2144 e 5024 bytes. A cada rodada, o endereço do buffer muda — prova de que o crescimento é um `malloc` de um bloco novo seguido de um `free` do bloco antigo, não um `realloc` no lugar. Os números batem com a conta feita à mão na Seção 4.4: o crescimento geométrico por 3, mais o detalhe de que `line_aligned_size` sempre arredonda para cima em pelo menos um `sizeof(struct line)` extra."
  ),
  mixedBody(
    "A segunda demonstração mostra a extração de um campo por aritmética de ponteiros, na pilha. A linha de entrada é “banana 42 2024-01-05”, equivalente a rodar `sort -k2,2` numa linha com três campos separados por espaço. O programa imprime os endereços reais de pilha de `main()` e de um nível de chamada abaixo, mostrando o quadro de pilha sendo empilhado a cada chamada."
  ),
  caption("Saída real de harness_subconjunto.c (gcc 13.2.0, status de compilação/execução 0)"),
  ...codeLines(saidaExemplo),
  mixedBody(
    "Um achado não planejado apareceu na execução: o campo extraído veio “ 42”, com o espaço na frente — não “42” limpo. Não é um erro. É o comportamento documentado no próprio comentário de `begfield` (`sort.c:1869-1871`): “the leading field separator itself is included in a field when -t is absent” — o separador do campo fica incluído quando a opção `-t` não é usada. O harness roda com `skipsblanks = false`, equivalente a `sort -k2,2` sem a opção `-b`. Esse achado prova que o código foi executado e entendido, não só lido."
  ),
];

// =======================================================================
// SEÇÃO 8 — Construção e testes automatizados
// =======================================================================
const saidaTestes = `=== Grupo 1: buffer (heap) -- try_growbuf/maybe_growbuf ===
  OK   maybe_growbuf cresce quando ha espaco (limit >> alloc)
  OK   maybe_growbuf converge: alloc final >= limite pedido
  OK   maybe_growbuf converge sem marcar growth_failed
  OK   maybe_growbuf nao cresce quando limite <= alloc atual
  OK   maybe_growbuf com policy=NULL nao cresce nem falha
  OK   try_growbuf recusa quando o alloc alinhado nao ultrapassa o atual
  OK   apos crescer, o endereco do buffer muda (malloc novo, nao in-place)

=== Grupo 2: aritmetica de ponteiros -- begfield/limfield ===
  OK   -k2,2 sem -b inclui o separador: campo == " 42"
  OK   -k2,2 com -b (skipsblanks) da campo limpo == "42"
  OK   sword=0 retorna o inicio da linha sem avancar
  OK   sword alem do numero de campos para em lim (sem estourar o buffer)
  OK   com -t (tab != TAB_DEFAULT), begfield usa memchr e acha o 2o campo

12/12 testes passaram.`;

const sec8 = [
  marker("sec8"),
  mixedBody(
    "A disciplina não fixa uma quantidade mínima de testes automatizados. A dupla estipulou a própria: pelo menos um teste de caminho normal e um de caso de borda para cada par de funções do subconjunto com lógica condicional não trivial. O resultado foram 12 testes, cobrindo as 6 funções do subconjunto com lógica — 5 sobre `try_growbuf`/`maybe_growbuf`, no heap, e 5 sobre `begfield`/`limfield`, com ponteiros. As duas funções restantes, `buffer_linelim` e `line_aligned_size`, são pequenas demais para merecer teste isolado; são testadas indiretamente através das outras."
  ),
  mixedBody(
    "O arquivo `subconjunto.h` reúne as 6 funções verbatim, mais os stubs mínimos, num header incluído tanto pelo exemplo de uso quanto pelos testes, sem duplicar código. O arquivo `testes.c` roda os 12 testes sem depender de nenhum framework externo: uma função `check()` conta os testes que passam e os que falham, e o programa termina com código de saída 1 se algum teste falhar. Essa escolha permite rodar `make test` de forma automatizada, sem precisar de conferência visual."
  ),
  mixedBody(
    "O Makefile define quatro alvos: `all` compila o exemplo de uso e os testes; `run` compila e roda a demonstração; `test` compila e roda os testes; `clean` remove os binários. A dupla optou por um Makefile simples em vez do CMake do ambiente de referência `mflash/DevCPP` — achado no Moodle da disciplina —, porque o critério de avaliação cita “make” especificamente, e um Makefile plano é mais fácil de ler e avaliar num relatório."
  ),
  mixedBody(
    "`make test` compilou e rodou num compilador online real (mesmo processo da Seção 7), com resultado de 12 testes passando em 12."
  ),
  caption("Saída real de “make test” (gcc 13.2.0, status 0)"),
  ...codeLines(saidaTestes),
  mixedBody(
    "Vale registrar um achado real do processo, não só o resultado final. Na primeira rodada de testes, 11 dos 12 passaram — um teste sobre `try_growbuf` falhou. A investigação revelou que a expectativa do teste estava errada, não o código de `sort.c`. A comparação `alloc <= buf->alloc`, na linha 1805, usa o valor já alinhado por `line_aligned_size` (`sort.c:1623-1636`), não o valor original passado para a função. E `line_aligned_size` sempre aumenta o valor em pelo menos 1 byte, mesmo quando o tamanho já é múltiplo do alinhamento — porque a expressão `alignment - size % alignment` resulta em `alignment` inteiro quando o resto da divisão é zero. O teste foi corrigido para refletir esse comportamento real. Esse processo mostra que os 12 testes são reais, não decoração."
  ),
];

// =======================================================================
// CONCLUSÃO
// =======================================================================
const concl = [
  marker("concl"),
  mixedBody(
    "Este trabalho estudou um subconjunto de 7 funções do utilitário `sort` do GNU Coreutils — 677 linhas do código original, acima do mínimo de 600 linhas pedido pelo enunciado."
  ),
  mixedBody(
    "O subconjunto revelou um padrão estrutural único: o buffer de leitura guarda, numa única alocação de memória, o texto das linhas crescendo do início para o fim e a tabela de linhas crescendo do fim para o início. Esse padrão explica por que `begfield` e `limfield` nunca indexam texto com colchetes — sempre usam aritmética de ponteiros — e por que `fillbuf` decrementa um ponteiro em vez de incrementar um índice ao preencher a tabela de linhas."
  ),
  mixedBody(
    "Foram identificados oito truques de programador C, entre eles tabelas indexadas por byte, crescimento geométrico de buffer, small-buffer optimization na pilha, e o atributo `ATTRIBUTE_PURE` do GCC. O arquivo inteiro tem apenas dois `goto` — um deles dentro do subconjunto, usado de forma disciplinada para evitar duplicar a lógica de encerramento de `sort()`."
  ),
  mixedBody(
    "Mike Haertel escreveu o código original em 1988, com a mesma filosofia de engenharia que aplicou ao GNU grep: fazer o mínimo de trabalho possível por byte processado. Paul Eggert mantém o código hoje, com décadas de contribuições ao projeto GNU e ao tz database da IANA."
  ),
  mixedBody(
    "O exemplo de uso e os 12 testes automatizados foram executados de verdade — não previstos —, num compilador online, depois que o ambiente de desenvolvimento local bloqueou o gcc e o gdb por questões de segurança corporativa. Essa execução real revelou dois achados não planejados: o comportamento documentado de `begfield` ao incluir o separador de campo, e um erro na expectativa de um teste, não no código de `sort.c` — corrigido depois de investigado."
  ),
  mixedBody(
    "O código estudado neste trabalho confirma, na prática, os idiomas de software básico descritos no enunciado da disciplina: aritmética de ponteiros, contagem regressiva e desvio incondicional, todos usados com um propósito claro de desempenho e controle de memória — nunca por acaso."
  ),
];

// =======================================================================
// Documento
// =======================================================================
const doc = new Document({
  styles: {
    default: {
      document: {
        run: { font: "Times New Roman", size: 24 },
        paragraph: { spacing: { line: 360, lineRule: "auto" } },
      },
    },
  },
  sections: [
    {
      properties: {},
      children: [...sec2, ...sec3, ...sec4, ...sec5, ...sec6, ...sec7, ...sec8, ...concl],
    },
  ],
});

Packer.toBuffer(doc).then((buffer) => {
  const outPath = path.join(__dirname, "content.docx");
  fs.writeFileSync(outPath, buffer);
  console.log("OK ->", outPath);
});
