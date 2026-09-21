import re

DOC = "../inspect/word/document.xml"

with open(DOC, encoding="utf-8") as f:
    content = f.read()

m = re.search(r"<w:tbl>.*?</w:tbl>", content, re.S)
assert m, "table not found"
tbl = m.group(0)

# ---------------------------------------------------------------------
# 1) Fix capitalization: "dispatcher" -> "Dispatcher"
# ---------------------------------------------------------------------
tbl = tbl.replace(">dispatcher mem", ">Dispatcher mem")

# ---------------------------------------------------------------------
# 2) Turn backtick-wrapped identifiers into real monospace runs (Consolas),
#    dropping the literal backticks. Handles a <w:t>...</w:t> that contains
#    one or more `code` spans, splitting it into multiple <w:r> runs.
# ---------------------------------------------------------------------
CODE_RPR = '<w:rPr><w:rFonts w:ascii="Consolas" w:hAnsi="Consolas" w:cs="Consolas"/></w:rPr>'


def split_backticks(text_run_xml):
    tm = re.search(r"<w:t>(.*?)</w:t>", text_run_xml, re.S)
    if not tm or "`" not in tm.group(1):
        return text_run_xml
    text = tm.group(1)
    parts = re.split(r"`([^`]+)`", text)  # alternates: plain, code, plain, code, ...
    runs = []
    for i, part in enumerate(parts):
        if part == "":
            continue
        if i % 2 == 1:
            runs.append(f"<w:r>{CODE_RPR}<w:t xml:space=\"preserve\">{part}</w:t></w:r>")
        else:
            runs.append(f"<w:r><w:rPr/><w:t xml:space=\"preserve\">{part}</w:t></w:r>")
    return "".join(runs)


def fix_cell_paragraph(match):
    run = match.group(0)
    if "`" not in run:
        return run
    new_runs = split_backticks(run)
    return new_runs


tbl = re.sub(r"<w:r><w:rPr/><w:t>[^<]*`[^<]*</w:t></w:r>", fix_cell_paragraph, tbl)

assert "`" not in tbl, "backticks remain: " + tbl[tbl.find("`") - 60 : tbl.find("`") + 60]

# ---------------------------------------------------------------------
# 3) Booktabs-style borders: top rule (thick) on row 1, header underline
#    (thin) on row 1, bottom rule (thick) on the last row, no vertical
#    borders and no borders at all on the interior rows.
# ---------------------------------------------------------------------
tbl = re.sub(r"<w:tcBorders>.*?</w:tcBorders>", "", tbl, flags=re.S)

rows = re.findall(r"<w:tr>.*?</w:tr>", tbl, re.S)
assert len(rows) == 8, f"expected 8 rows, found {len(rows)}"

TOP_RULE = '<w:tcBorders><w:top w:val="single" w:sz="12" w:space="0" w:color="000000"/></w:tcBorders>'
HEADER_RULE = '<w:tcBorders><w:top w:val="single" w:sz="12" w:space="0" w:color="000000"/><w:bottom w:val="single" w:sz="4" w:space="0" w:color="000000"/></w:tcBorders>'
BOTTOM_RULE = '<w:tcBorders><w:bottom w:val="single" w:sz="12" w:space="0" w:color="000000"/></w:tcBorders>'


def set_row_borders(row_xml, border_xml):
    # insert border_xml right after each <w:tcPr><w:tcW .../> in this row
    return re.sub(r"(<w:tcPr><w:tcW[^/]*/>)", r"\1" + border_xml, row_xml)


rows[0] = set_row_borders(rows[0], HEADER_RULE)
rows[-1] = set_row_borders(rows[-1], BOTTOM_RULE)

# ---------------------------------------------------------------------
# 4) Column widths: give Motivo more room (Função 22%, Linhas 12%, Motivo 66%),
#    done per-row by position (1st/2nd/3rd tcW in each row), since col 1 and
#    col 2 share the same original width value (3024) and a global replace
#    can't tell them apart.
# ---------------------------------------------------------------------
NEW_WIDTHS = ['1996', '1089', '5988']


def fix_widths(row_xml):
    counter = {"i": 0}

    def repl(mm):
        w = NEW_WIDTHS[counter["i"]]
        counter["i"] += 1
        return f'<w:tcW w:w="{w}" w:type="dxa"/>'

    return re.sub(r'<w:tcW w:w="\d+" w:type="dxa"/>', repl, row_xml)


rows = [fix_widths(r) for r in rows]
new_tbl_body = "".join(rows)

# splice rows back in (tblPr + tblGrid stay, followed by the fixed rows)
head_m = re.search(r"^(.*?)(<w:tr>)", tbl, re.S)
tbl = head_m.group(1) + new_tbl_body + "</w:tbl>"
tbl = tbl.replace('<w:gridCol w:w="3024"/><w:gridCol w:w="3024"/><w:gridCol w:w="3025"/>',
                   '<w:gridCol w:w="1996"/><w:gridCol w:w="1089"/><w:gridCol w:w="5988"/>')

# ---------------------------------------------------------------------
# write back
# ---------------------------------------------------------------------
content = content[: m.start()] + tbl + content[m.end() :]
with open(DOC, "w", encoding="utf-8") as f:
    f.write(content)

print("OK — table fixed:", len(tbl), "chars")
