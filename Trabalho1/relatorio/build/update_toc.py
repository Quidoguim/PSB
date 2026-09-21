"""Update a .docx's Table of Contents field (and other fields) without MS Word,
using LibreOffice's own Python + UNO bridge. Run with LibreOffice's bundled
python.exe, e.g.:

  "C:\\Program Files\\LibreOffice\\program\\python.exe" update_toc.py path\\to\\file.docx

Rerun this any time the report's headings or page count change and the
Sumário needs to reflect the new page numbers.
"""
import subprocess
import time
import sys
import os

DOC_PATH = os.path.abspath(sys.argv[1])
SOFFICE = r"C:\Program Files\LibreOffice\program\soffice.exe"

proc = subprocess.Popen([
    SOFFICE, "--headless", "--invisible", "--norestore",
    "--accept=socket,host=localhost,port=2002;urp;",
])

import uno
from com.sun.star.beans import PropertyValue


def prop(name, value):
    p = PropertyValue()
    p.Name = name
    p.Value = value
    return p


localContext = uno.getComponentContext()
resolver = localContext.ServiceManager.createInstanceWithContext(
    "com.sun.star.bridge.UnoUrlResolver", localContext
)

ctx = None
for _ in range(40):
    try:
        ctx = resolver.resolve(
            "uno:socket,host=localhost,port=2002;urp;StarOffice.ComponentContext"
        )
        break
    except Exception:
        time.sleep(0.5)
if ctx is None:
    raise RuntimeError("could not connect to soffice")

smgr = ctx.ServiceManager
desktop = smgr.createInstanceWithContext("com.sun.star.frame.Desktop", ctx)

url = uno.systemPathToFileUrl(DOC_PATH)
doc = desktop.loadComponentFromURL(url, "_blank", 0, (prop("Hidden", True),))

indexes = doc.getDocumentIndexes()
print("indexes found:", indexes.getCount())
for i in range(indexes.getCount()):
    idx = indexes.getByIndex(i)
    idx.update()
    print("updated index", i, idx.getName())

doc.getTextFields().refresh()

doc.store()
doc.close(False)

proc.terminate()
print("DONE")
