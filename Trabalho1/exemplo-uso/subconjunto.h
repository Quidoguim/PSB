/* subconjunto.h
 *
 * Header-only: tipos minimos, stubs de dependencias externas, e copia
 * VERBATIM de 6 funcoes do subconjunto estudado de sort.c (GNU Coreutils,
 * commit de referencia bff0e54 -- ver CLAUDE.md #codigo-fonte):
 * buffer_linelim, line_aligned_size, try_growbuf, maybe_growbuf,
 * begfield, limfield. Nenhuma logica foi alterada -- so os tipos/stubs
 * ao redor, para poder compilar isolado (sort.c completo depende da
 * arvore de build inteira do coreutils/gnulib).
 *
 * Incluido tanto por demo.c (exemplo de uso, passo 11) quanto por
 * testes.c (testes automatizados, passo 12) -- todas as funcoes aqui
 * sao 'static', entao incluir este header em mais de uma unidade de
 * traducao nao gera erro de simbolo duplicado (idioma padrao de C
 * pra header-only).
 */

#ifndef SUBCONJUNTO_H
#define SUBCONJUNTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

/* ---------------------------------------------------------------------
 * HARNESS: tipos minimos, so com os campos que as funcoes copiadas usam.
 * Nomes de campo identicos aos de sort.c para que o codigo verbatim
 * abaixo compile sem qualquer edicao.
 * --------------------------------------------------------------------- */

struct line
{
  char *text;
  size_t length;
  char *keybeg;
  char *keylim;
};

struct keyfield
{
  size_t sword, schar, eword, echar;
  bool skipsblanks, skipeblanks;
};

struct buffer
{
  char *buf;
  size_t used, alloc, left, nlines, line_bytes;
};

struct sort_buffer_policy
{
  size_t initial, limit;
  bool growth_failed;
};

/* HARNESS: stubs de dependencias externas de sort.c (definidas em
 * system.h/gnulib no arquivo real, fora do nosso subconjunto). */

#define ATTRIBUTE_PURE __attribute__((__pure__))
#define UCHAR_LIM (UCHAR_MAX + 1)

static inline unsigned char
to_uchar (char ch) { return ch; }

static bool blanks[UCHAR_LIM];

enum { TAB_DEFAULT = CHAR_MAX + 1 };
static int tab = TAB_DEFAULT;

static struct keyfield *keylist = NULL; /* sem -k ativo no realloc de try_growbuf */

/* stub simplificado de ckd_add (checked-arithmetic macro do C23/gnulib);
 * nao verificamos overflow aqui porque os tamanhos do exemplo/testes sao
 * pequenos e conhecidos. */
static inline bool
ckd_add (size_t *r, size_t a, size_t b) { *r = a + b; return false; }

static void
init_blanks (void)
{
  for (size_t i = 0; i < UCHAR_LIM; ++i)
    blanks[i] = (i == '\n') || (i == ' ') || (i == '\t');
}

/* =======================================================================
 * A PARTIR DAQUI: codigo verbatim de sort.c (GNU Coreutils, commit bff0e54)
 * ======================================================================= */

/* sort.c:1791-1796 */
static inline struct line *
buffer_linelim (struct buffer const *buf)
{
  void *linelim = buf->buf + buf->alloc;
  return linelim;
}

/* sort.c:1623-1636 */
static bool
line_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = sizeof (struct line);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  if (ckd_add (&aligned, size, padding))
    return false;

  *alloc = aligned;
  return true;
}

/* sort.c:1802-1836 */
static bool
try_growbuf (struct buffer *buf, size_t alloc)
{
  if (! line_aligned_size (&alloc) || alloc <= buf->alloc)
    return false;

  char *newbuf = malloc (alloc);
  if (! newbuf)
    return false;

  char *oldbuf = buf->buf;
  struct line *old_linelim = buffer_linelim (buf);
  struct line *old_line = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  struct line *new_linelim = (void *) (newbuf + alloc);
  struct line *new_line = new_linelim - buf->nlines;
  memcpy (new_line, old_line, buf->nlines * sizeof *new_line);

  for (struct line *line = new_line; line < new_linelim; line++)
    {
      line->text = newbuf + (line->text - oldbuf);
      if (keylist)
        {
          line->keybeg = newbuf + (line->keybeg - oldbuf);
          line->keylim = newbuf + (line->keylim - oldbuf);
        }
    }

  free (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  return true;
}

/* sort.c:1840-1857 */
static bool
maybe_growbuf (struct buffer *buf, struct sort_buffer_policy *policy)
{
  if (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    return false;

  size_t alloc;
  if (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  else
    alloc = policy->limit;

  if (try_growbuf (buf, alloc))
    return true;

  policy->growth_failed = true;
  return false;
}

/* sort.c:1862-1903 */
static char *
begfield (struct line const *line, struct keyfield const *key)
{
  char *ptr = line->text, *lim = ptr + line->length - 1;
  size_t sword = key->sword;
  size_t schar = key->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  if (tab != TAB_DEFAULT)
    while (ptr < lim && sword--)
      {
        char *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        if (ptr < lim)
          ++ptr;
      }
  else
    while (ptr < lim && sword--)
      {
        while (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        while (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  if (key->skipsblanks)
    while (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  if (schar < remaining_bytes)
    ptr += schar;
  else
    ptr = lim;

  return ptr;
}

/* sort.c:1908-2010 (inclui o ATTRIBUTE_PURE, ver Truques de programador C) */
ATTRIBUTE_PURE
static char *
limfield (struct line const *line, struct keyfield const *key)
{
  char *ptr = line->text, *lim = ptr + line->length - 1;
  size_t eword = key->eword, echar = key->echar;

  if (echar == 0)
    eword++; /* Skip all of end field.  */

  if (tab != TAB_DEFAULT)
    while (ptr < lim && eword--)
      {
        char *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        if (ptr < lim && (eword || echar))
          ++ptr;
      }
  else
    while (ptr < lim && eword--)
      {
        while (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        while (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  if (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      if (key->skipeblanks)
        while (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      size_t remaining_bytes = lim - ptr;
      if (echar < remaining_bytes)
        ptr += echar;
      else
        ptr = lim;
    }

  return ptr;
}

#endif /* SUBCONJUNTO_H */
