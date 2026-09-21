const fs = require("fs");
const path = require("path");
const {
  Document, Packer, Paragraph, TextRun, HeadingLevel, AlignmentType,
  TableOfContents, PageBreak, BorderStyle, ShadingType, LevelFormat,
  convertInchesToTwip, Tab, PositionalTab, PositionalTabAlignment, PositionalTabLeader,
} = require("docx");

// ---------------------------------------------------------------------
// Cores/medidas
// ---------------------------------------------------------------------
const CODE_BG = "F2F2F2";
const RULE_COLOR = "808080";

// ---------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------
function heading(text, level, numbering) {
  return new Paragraph({
    text,
    heading: level,
    spacing: { before: 320, after: 160 },
  });
}

function body(text, opts = {}) {
  return new Paragraph({
    children: [new TextRun({ text, italics: !!opts.italics, bold: !!opts.bold })],
    alignment: AlignmentType.JUSTIFIED,
    spacing: { after: 200, line: 360, lineRule: "auto" }, // 1.5 linhas
    indent: opts.firstLine ? { firstLine: 700 } : undefined,
  });
}

function placeholder(label) {
  return new Paragraph({
    children: [new TextRun({ text: `[ ${label} ]`, italics: true, color: "AA0000" })],
    spacing: { after: 240, before: 120 },
    shading: { type: ShadingType.CLEAR, fill: "FFF3F3" },
    border: {
      top: { style: BorderStyle.SINGLE, size: 4, color: "AA0000" },
      bottom: { style: BorderStyle.SINGLE, size: 4, color: "AA0000" },
      left: { style: BorderStyle.SINGLE, size: 4, color: "AA0000" },
      right: { style: BorderStyle.SINGLE, size: 4, color: "AA0000" },
    },
  });
}

function caption(text) {
  return new Paragraph({
    children: [new TextRun({ text, italics: true, size: 20 })],
    alignment: AlignmentType.CENTER,
    spacing: { before: 200, after: 80 },
  });
}

function codeBlockFromFile(filePath) {
  const lines = fs.readFileSync(filePath, "utf8").replace(/\r\n/g, "\n").split("\n").filter((l) => l.length > 0);
  const paras = lines.map(
    (line) =>
      new Paragraph({
        children: [new TextRun({ text: line, font: "Consolas", size: 18 })],
        spacing: { after: 0, line: 240, lineRule: "auto" },
        shading: { type: ShadingType.CLEAR, fill: CODE_BG },
      })
  );
  // borda superior na primeira linha, inferior na ultima (efeito "caixa" como no modelo)
  paras[0].properties = paras[0].properties || {};
  return paras;
}

function refEntry(text) {
  return new Paragraph({
    children: [new TextRun({ text })],
    spacing: { after: 200, line: 240, lineRule: "auto" },
    indent: { hanging: 500, left: 500 },
  });
}

// ---------------------------------------------------------------------
// Anexo: 7 blocos de codigo
// ---------------------------------------------------------------------
const CB = (n) => path.join(__dirname, "codeblocks", n);
const anexoBlocos = [
  { file: "1_try_growbuf_maybe_growbuf.txt", nome: "try_growbuf / maybe_growbuf", linhas: "sort.c:1802–1857" },
  { file: "2_begfield.txt", nome: "begfield", linhas: "sort.c:1862–1903" },
  { file: "3_limfield.txt", nome: "limfield", linhas: "sort.c:1908–2010" },
  { file: "4_fillbuf.txt", nome: "fillbuf", linhas: "sort.c:2018–2133" },
  { file: "5_keycompare.txt", nome: "keycompare", linhas: "sort.c:2946–3136" },
  { file: "6_compare.txt", nome: "compare", linhas: "sort.c:3141–3181" },
  { file: "7_sort.txt", nome: "sort()", linhas: "sort.c:4314–4441" },
];

let anexoParas = [];
anexoBlocos.forEach((b, i) => {
  anexoParas.push(caption(`Código-fonte ${i + 1}: ${b.nome} (${b.linhas})`));
  anexoParas = anexoParas.concat(codeBlockFromFile(CB(b.file)));
});

// ---------------------------------------------------------------------
// Documento
// ---------------------------------------------------------------------
const doc = new Document({
  styles: {
    default: {
      document: {
        run: { font: "Times New Roman", size: 24 }, // 12pt
        paragraph: { spacing: { line: 360, lineRule: "auto" } }, // 1.5
      },
    },
    paragraphStyles: [
      {
        id: "Heading1",
        name: "Heading 1",
        basedOn: "Normal",
        next: "Normal",
        quickFormat: true,
        run: { font: "Times New Roman", size: 32, bold: true },
        paragraph: { spacing: { before: 480, after: 240 }, outlineLevel: 0 },
      },
      {
        id: "Heading2",
        name: "Heading 2",
        basedOn: "Normal",
        next: "Normal",
        quickFormat: true,
        run: { font: "Times New Roman", size: 27, bold: true },
        paragraph: { spacing: { before: 360, after: 200 }, outlineLevel: 1 },
      },
      {
        id: "Heading3",
        name: "Heading 3",
        basedOn: "Normal",
        next: "Normal",
        quickFormat: true,
        run: { font: "Times New Roman", size: 24, bold: true, italics: true },
        paragraph: { spacing: { before: 280, after: 160 }, outlineLevel: 2 },
      },
    ],
  },
  sections: [
    {
      properties: {
        page: {
          margin: {
            top: convertInchesToTwip(3 / 2.54),
            left: convertInchesToTwip(3 / 2.54),
            bottom: convertInchesToTwip(2 / 2.54),
            right: convertInchesToTwip(2 / 2.54),
          },
        },
      },
      children: [
        // ---------------- Capa ----------------
        new Paragraph({ text: "", spacing: { before: 2000 } }),
        new Paragraph({
          children: [new TextRun({ text: "PUCRS — Escola Politécnica", size: 24 })],
          alignment: AlignmentType.CENTER,
        }),
        new Paragraph({
          children: [new TextRun({ text: "Programação de Software Básico", size: 24 })],
          alignment: AlignmentType.CENTER,
          spacing: { after: 600 },
        }),
        new Paragraph({
          children: [new TextRun({ text: "Análise do utilitário sort.c", size: 36, bold: true })],
          alignment: AlignmentType.CENTER,
          spacing: { after: 200 },
        }),
        new Paragraph({
          children: [new TextRun({ text: "(GNU Coreutils)", size: 24, italics: true })],
          alignment: AlignmentType.CENTER,
          spacing: { after: 800 },
        }),
        new Paragraph({
          children: [new TextRun({ text: "[ Nome completo — Integrante 1 ]", size: 24 })],
          alignment: AlignmentType.CENTER,
        }),
        new Paragraph({
          children: [new TextRun({ text: "[ Nome completo — Integrante 2 ]", size: 24 })],
          alignment: AlignmentType.CENTER,
          spacing: { after: 800 },
        }),
        new Paragraph({
          children: [new TextRun({ text: "Trabalho 1 — Setembro de 2026", size: 24 })],
          alignment: AlignmentType.CENTER,
        }),
        new Paragraph({ children: [new PageBreak()] }),

        // ---------------- Sumario ----------------
        new Paragraph({ text: "Sumário", heading: HeadingLevel.HEADING_1 }),
        new TableOfContents("Sumário", {
          hyperlink: true,
          headingStyleRange: "1-3",
        }),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= 1. Introducao =================
        heading("1. Introdução", HeadingLevel.HEADING_1),
        body(
          "O utilitário sort ordena as linhas de um ou mais arquivos de texto. Este documento estuda a implementação de sort no GNU Coreutils, o pacote de utilitários básicos do sistema GNU. O objetivo é reconhecer, no código real de um programador profissional, os idiomas e as técnicas típicas da programação em linguagem C no contexto de software básico."
        ),
        heading("1.1 Código-fonte", HeadingLevel.HEADING_2),
        body(
          "O arquivo sort.c tem 5154 linhas. O texto vem do repositório oficial do GNU Coreutils, branch master, commit bff0e54 (\"build: sort: explicitly tag libcrypto dependency\", 6 de setembro de 2026). Uma cópia do arquivo está no repositório deste trabalho."
        ),
        body(
          "O arquivo inteiro é grande demais para uma apresentação de 10 minutos. Por isso, este documento cobre um subconjunto de 7 funções, com 677 linhas de código original — acima do mínimo de 600 linhas pedido pelo enunciado."
        ),
        body(
          "As 7 funções formam um fluxo único, do início ao fim: leitura do arquivo de entrada em blocos (fillbuf), extração da chave de ordenação de cada linha por aritmética de ponteiros (begfield, limfield), crescimento do buffer de leitura quando necessário (try_growbuf, maybe_growbuf), comparação entre linhas (compare, keycompare) e o despacho entre ordenação em memória e ordenação externa por arquivos temporários (sort). A Tabela 1 lista as 7 funções com a linha de início e fim de cada uma no arquivo original."
        ),
        placeholder("Inserir aqui a Tabela 1 (função · linhas · motivo da escolha) — texto pronto em CLAUDE.md, seção \"Código-fonte\""),
        heading("1.2 Estrutura deste texto", HeadingLevel.HEADING_2),
        body(
          "A Seção 2 apresenta a carreira dos dois autores do código. A Seção 3 descreve as convenções de formatação do projeto GNU. A Seção 4 apresenta os idiomas e truques de linguagem C encontrados no subconjunto. A Seção 5 divide o subconjunto em blocos de responsabilidade e lista as suas dependências. A Seção 6 mostra o cenário principal de execução, com um diagrama estático e um diagrama dinâmico. A Seção 7 apresenta um exemplo de uso real, com endereços de pilha e de heap. A Seção 8 descreve a construção e os testes automatizados do subconjunto. O texto termina com a conclusão, as referências e o anexo com o código-fonte completo do subconjunto."
        ),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= 2. Historico dos autores =================
        heading("2. Histórico dos autores", HeadingLevel.HEADING_1),
        placeholder(
          "CONTEÚDO A DESENVOLVER (próxima mensagem) — Mike Haertel e Paul Eggert. Fontes já levantadas em CLAUDE.md, seção \"Histórico dos autores\". Sem figura necessária nesta seção."
        ),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= 3. Convencoes =================
        heading("3. Convenções de codificação", HeadingLevel.HEADING_1),
        placeholder(
          "CONTEÚDO A DESENVOLVER — convenções de identação, chaves, espaçamento, identificadores (GNU Coding Standards). Fonte: CLAUDE.md, seção \"Convenções de codificação\". Sem figura necessária; pode citar 2–3 trechos curtos de código inline (não precisa de bloco separado)."
        ),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= 4. Idiomas =================
        heading("4. Idiomas e truques de programador C", HeadingLevel.HEADING_1),
        heading("4.1 Aritmética de ponteiros", HeadingLevel.HEADING_2),
        placeholder(
          "CONTEÚDO A DESENVOLVER — begfield/limfield/fillbuf. Fonte: CLAUDE.md, seção \"Aritmética de ponteiros\". Inserir trecho de código de begfield (Código-fonte do Anexo 2) como ilustração."
        ),
        heading("4.2 Contagem regressiva", HeadingLevel.HEADING_2),
        placeholder("CONTEÚDO A DESENVOLVER — while (ptr < lim && sword--), mesmo idioma citado no modelo do echo.c."),
        heading("4.3 Desvio incondicional", HeadingLevel.HEADING_2),
        placeholder("CONTEÚDO A DESENVOLVER — o único goto do subconjunto, em sort() (sort.c:4418, label finish)."),
        heading("4.4 Outros truques", HeadingLevel.HEADING_2),
        placeholder(
          "CONTEÚDO A DESENVOLVER — buffer geométrico, small-buffer optimization, do-while(0), ATTRIBUTE_PURE, tabelas indexadas por byte. Fonte: CLAUDE.md, seção \"Truques de programador C\" (8 itens já levantados, escolher os mais fortes pra não sobrecarregar o vídeo)."
        ),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= 5. Blocos =================
        heading("5. Divisão em blocos", HeadingLevel.HEADING_1),
        heading("5.1 Dependências", HeadingLevel.HEADING_2),
        placeholder(
          "CONTEÚDO A DESENVOLVER — tabela de dependências internas/externas. Fonte: CLAUDE.md, seção \"Blocos de responsabilidade e dependências\". Bom lugar pra citar Knuth (TAOCP Vol.3, Seção 5.4) ao mencionar a maquinaria de merge externo que fica fora do subconjunto — fecha o critério de referência acadêmica de forma natural, sem seção separada."
        ),
        heading("5.2 Blocos propostos", HeadingLevel.HEADING_2),
        placeholder("CONTEÚDO A DESENVOLVER — tabela de sub-blocos por função, mesmo formato da Tabela 2 do modelo."),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= 6. Cenario principal =================
        heading("6. Cenário principal", HeadingLevel.HEADING_1),
        placeholder("CONTEÚDO A DESENVOLVER — breve texto de transição antes dos diagramas."),
        heading("6.1 Diagrama estático", HeadingLevel.HEADING_2),
        placeholder(
          "INSERIR FIGURA — diagrama-estatico-sort.svg (converter para PNG antes de colar). Legenda: \"Figura 1: Grafo de chamadas do subconjunto estudado\"."
        ),
        heading("6.2 Diagrama dinâmico", HeadingLevel.HEADING_2),
        placeholder(
          "INSERIR FIGURA — diagrama-dinamico-sort.svg (converter para PNG antes de colar). Legenda: \"Figura 2: Fluxo de execução de sort()\"."
        ),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= 7. Exemplo de uso =================
        heading("7. Exemplo de uso", HeadingLevel.HEADING_1),
        placeholder(
          "CONTEÚDO A DESENVOLVER — execução real via compilador online (gdb local bloqueado pelo ambiente). Fonte: CLAUDE.md, seção \"Exemplo de uso\", e Trabalho1/exemplo-uso/saida-exemplo.txt. Pode inserir trecho da saída real como bloco de código."
        ),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= 8. Testes =================
        heading("8. Construção e testes automatizados", HeadingLevel.HEADING_1),
        placeholder(
          "CONTEÚDO A DESENVOLVER — Makefile e os 12 testes automatizados (12/12 passando). Fonte: CLAUDE.md, seção \"Construção e testes automatizados\", e Trabalho1/exemplo-uso/saida-testes.txt. Vale contar o achado real (teste com expectativa errada, corrigido) — mostra que os testes são reais."
        ),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= Conclusao =================
        heading("Conclusão", HeadingLevel.HEADING_1),
        placeholder("A ESCREVER POR ÚLTIMO — depois que todas as seções acima estiverem prontas, resume os achados principais em 1 parágrafo curto."),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= Referencias =================
        heading("Referências", HeadingLevel.HEADING_1),
        refEntry(
          "GNU Coreutils (2026). Código-fonte do utilitário sort.c. Commit bff0e54. Acesso em setembro de 2026. url: https://github.com/coreutils/coreutils/blob/master/src/sort.c."
        ),
        refEntry(
          "Haertel, Mike (2010). \"why GNU grep is fast\". Lista freebsd-current, 21 ago. 2010. url: https://lists.freebsd.org/pipermail/freebsd-current/2010-August/019310.html."
        ),
        refEntry(
          "Eggert, Paul; Parker, Douglas Stott Jr. (1993). \"File Systems in User Space\". USENIX Winter 1993 Technical Conference."
        ),
        refEntry(
          "Eggert, Paul et al. (2019). RFC 8536 — \"The Time Zone Information Format (TZif)\". IETF. doi: 10.17487/RFC8536."
        ),
        refEntry(
          "Knuth, Donald E. (1998). The Art of Computer Programming, Volume 3: Sorting and Searching. 2ª ed. Addison-Wesley. Seção 5.4, \"External Sorting\". isbn: 978-0-201-89685-5."
        ),
        refEntry(
          "Mayrhauser, Anneliese; Vans, A. Marie (set. de 1995). \"Program comprehension during software maintenance and evolution\". Em: Computer 28, pp. 44–55. doi: 10.1109/2.402076."
        ),
        new Paragraph({ children: [new PageBreak()] }),

        // ================= Anexo =================
        heading("Anexo — Código-fonte do subconjunto estudado", HeadingLevel.HEADING_1),
        body(
          "As 7 funções abaixo são o texto original de sort.c (GNU Coreutils, commit bff0e54), sem nenhuma alteração. Os números à esquerda são os números de linha do arquivo original."
        ),
        ...anexoParas,
      ],
    },
  ],
});

Packer.toBuffer(doc).then((buffer) => {
  const outPath = path.join(__dirname, "..", "Relatorio-T1-sort.c-esqueleto.docx");
  fs.writeFileSync(outPath, buffer);
  console.log("OK ->", outPath);
});
