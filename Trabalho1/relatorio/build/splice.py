import re
import os
import shutil

MAIN = "main_unzip"
CONTENT = "content_unzip"
MARK = "SPLIT"

# ---------------------------------------------------------------------
# 1) Load content.docx body, split into named chunks by marker paragraphs
# ---------------------------------------------------------------------
with open(f"{CONTENT}/word/document.xml", encoding="utf-8") as f:
    content_xml = f.read()

body_m = re.search(r"<w:body>(.*)</w:body>", content_xml, re.S)
body = body_m.group(1)
# strip the trailing sectPr (page setup) docx-js appends at end of body
body = re.sub(r"<w:sectPr\b.*?</w:sectPr>\s*$", "", body, flags=re.S)

paras = re.findall(r"<w:p\b.*?</w:p>", body, re.S)
chunks = {}
current_name = None
current_paras = []
for p in paras:
    mm = re.search(re.escape(MARK) + r"([a-zA-Z0-9\-]+)", p)
    if mm:
        if current_name is not None:
            chunks[current_name] = "".join(current_paras)
        current_name = mm.group(1)
        current_paras = []
    else:
        current_paras.append(p)
if current_name is not None:
    chunks[current_name] = "".join(current_paras)

print("Chunks extracted:", list(chunks.keys()))
for k, v in chunks.items():
    assert v.strip(), f"chunk {k} is empty!"

# fix image relationship ids: content.docx used rId7 (estatico) / rId8 (dinamico);
# main doc will get them as rId5 / rId6.
for name in ("sec6-1", "sec6-2"):
    assert 'r:embed="rId7"' in chunks[name] or 'r:embed="rId8"' in chunks[name] or True
chunks["sec6-1"] = chunks["sec6-1"].replace('r:embed="rId7"', 'r:embed="rId5"')
chunks["sec6-2"] = chunks["sec6-2"].replace('r:embed="rId8"', 'r:embed="rId6"')

# ---------------------------------------------------------------------
# 2) Load main document.xml, replace each placeholder paragraph
# ---------------------------------------------------------------------
with open(f"{MAIN}/word/document.xml", encoding="utf-8") as f:
    main_xml = f.read()

# (marker substring, chunk name)
targets = [
    ("CONTEÚDO A DESENVOLVER (próxima mensagem) — Mike Haertel e Paul Eggert", "sec2"),
    ("CONTEÚDO A DESENVOLVER — convenções de identação, chaves, espaçamento", "sec3"),
    ("CONTEÚDO A DESENVOLVER — begfield/limfield/fillbuf. Fonte", "sec4"),
    ("CONTEÚDO A DESENVOLVER — while (ptr &lt; lim &amp;&amp; sword--)", "sec4-2"),
    ("CONTEÚDO A DESENVOLVER — o único goto do subconjunto", "sec4-3"),
    ("CONTEÚDO A DESENVOLVER — buffer geométrico, small-buffer optimization", "sec4-4"),
    ("CONTEÚDO A DESENVOLVER — tabela de dependências internas/externas", "sec5-1"),
    ("CONTEÚDO A DESENVOLVER — tabela de sub-blocos por função", "sec5-2"),
    ("CONTEÚDO A DESENVOLVER — breve texto de transição antes dos diagramas", "sec6"),
    ("INSERIR FIGURA — diagrama-estatico-sort.svg", "sec6-1"),
    ("INSERIR FIGURA — diagrama-dinamico-sort.svg", "sec6-2"),
    ("CONTEÚDO A DESENVOLVER — execução real via compilador online", "sec7"),
    ("CONTEÚDO A DESENVOLVER — Makefile e os 12 testes automatizados", "sec8"),
    ("A ESCREVER POR ÚLTIMO — depois que todas as seções", "concl"),
]

for marker_text, chunk_name in targets:
    # find the <w:p ...>...</w:p> that contains marker_text
    pattern = re.compile(r"<w:p\b(?:(?!</w:p>).)*?" + re.escape(marker_text) + r"(?:(?!</w:p>).)*?</w:p>", re.S)
    m = pattern.search(main_xml)
    assert m, f"placeholder not found for: {marker_text!r}"
    main_xml = main_xml[: m.start()] + chunks[chunk_name] + main_xml[m.end() :]
    print(f"spliced {chunk_name:10s} ({len(chunks[chunk_name])} chars) replacing placeholder {marker_text[:40]!r}")

with open(f"{MAIN}/word/document.xml", "w", encoding="utf-8") as f:
    f.write(main_xml)

# ---------------------------------------------------------------------
# 3) copy media files, add relationships for the 2 images
# ---------------------------------------------------------------------
os.makedirs(f"{MAIN}/word/media", exist_ok=True)
shutil.copy(f"{CONTENT}/word/media/baa308cbff1e10af5ce402005cf3a23cf8150b4e.png", f"{MAIN}/word/media/diagrama-estatico.png")
shutil.copy(f"{CONTENT}/word/media/08e0e85d610b70d8ee61a3ea87aec97b6d4b3bd4.png", f"{MAIN}/word/media/diagrama-dinamico.png")

with open(f"{MAIN}/word/_rels/document.xml.rels", encoding="utf-8") as f:
    rels = f.read()
assert "rId5" not in rels and "rId6" not in rels
new_rels = (
    '<Relationship Id="rId5" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image" Target="media/diagrama-estatico.png"/>'
    '<Relationship Id="rId6" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image" Target="media/diagrama-dinamico.png"/>'
)
rels = rels.replace("</Relationships>", new_rels + "</Relationships>")
with open(f"{MAIN}/word/_rels/document.xml.rels", "w", encoding="utf-8") as f:
    f.write(rels)

print("Done.")
