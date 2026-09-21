/* harness_subconjunto.c
 *
 * Exemplo de uso do subconjunto estudado de sort.c (passo 11 do roteiro,
 * CLAUDE.md). Não é gdb interativo: sem gdb disponível no ambiente (rede
 * corporativa bloqueou o download do pacote via pacman/MSYS2 por causa de
 * um proxy TLS), a ferramenta equivalente usada aqui é instrumentação
 * printf() sobre endereços reais de pilha (stack) e heap, que é uma forma
 * igualmente válida e reproduzível de observar consumo de memória.
 *
 * As cinco funções abaixo são CÓPIA VERBATIM de sort.c (GNU Coreutils,
 * commit de referência bff0e54 — ver CLAUDE.md #código-fonte), sem
 * nenhuma alteração de lógica: buffer_linelim, line_aligned_size,
 * try_growbuf, maybe_growbuf, begfield, limfield. O resto do arquivo
 * (structs mínimas, stubs de dependências externas, e a função main) é
 * harness nosso, escrito para poder compilar e exercitar essas cinco
 * funções isoladamente, já que sort.c completo depende de dezenas de
 * cabeçalhos internos do GNU Coreutils/gnulib (config.h, system.h,
 * argmatch.h, etc.) que não fazem parte do subconjunto estudado.
 *
 * Compilar:  gcc -O0 -g -Wall -o harness_subconjunto harness_subconjunto.c
 * Rodar:     ./harness_subconjunto
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

/* ---------------------------------------------------------------------
 * HARNESS: tipos mínimos, só com os campos que as funções copiadas usam.
 * Nomes de campo idênticos aos de sort.c para que o código verbatim
 * abaixo compile sem qualquer edição.
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

/* HARNESS: stubs de dependências externas de sort.c (definidas em
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
 * não verificamos overflow aqui porque os tamanhos do exemplo são
 * pequenos e conhecidos — na sort.c real, ckd_add detectaria overflow
 * de size_t antes de um malloc gigante acidental. */
static inline bool
ckd_add (size_t *r, size_t a, size_t b) { *r = a + b; return false; }

/* =======================================================================
 * A PARTIR DAQUI: código verbatim de sort.c (GNU Coreutils, commit bff0e54)
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

/* =======================================================================
 * A PARTIR DAQUI: harness nosso (main + funções auxiliares do exemplo)
 * ======================================================================= */

static void
init_blanks (void)
{
  for (size_t i = 0; i < UCHAR_LIM; ++i)
    blanks[i] = (i == '\n') || (i == ' ') || (i == '\t');
}

/* Chama begfield()/limfield() um nível de pilha abaixo de main(), só pra
 * deixar visível no print a diferença de endereço entre os dois quadros
 * (stack cresce "pra baixo" em x86-64: endereços menores = mais fundo
 * na pilha de chamadas). */
static void
demo_extrair_campo (struct line const *line, struct keyfield const *key,
                     int marcador_desta_pilha)
{
  printf ("  [dentro de demo_extrair_campo, 1 nivel abaixo de main]\n");
  printf ("  endereco de uma variavel local aqui: %p\n", (void *) &marcador_desta_pilha);

  char *inicio = begfield (line, key);
  char *fim = limfield (line, key);

  printf ("  begfield() -> %p  (offset %td bytes desde line.text)\n",
          (void *) inicio, inicio - line->text);
  printf ("  limfield()  -> %p  (offset %td bytes desde line.text)\n",
          (void *) fim, fim - line->text);
  printf ("  campo extraido (chave -k2,2): \"%.*s\"\n",
          (int) (fim - inicio), inicio);
}

int
main (void)
{
  init_blanks ();

  puts ("=== Demonstracao 1: crescimento do buffer (heap), try_growbuf/maybe_growbuf ===");
  puts ("(sort.c:1802-1857 -- ver 'Truques de programador C' no CLAUDE.md)\n");

  struct buffer buf = { .buf = malloc (64), .alloc = 64 };
  struct sort_buffer_policy policy = { .initial = 64, .limit = 5000, .growth_failed = false };

  printf ("buffer inicial : alloc=%6zu bytes  endereco=%p\n", buf.alloc, (void *) buf.buf);

  int rodada = 1;
  while (maybe_growbuf (&buf, &policy))
    {
      printf ("crescimento %d : alloc=%6zu bytes  endereco=%p  (buffer antigo foi liberado)\n",
              rodada++, buf.alloc, (void *) buf.buf);
    }

  printf ("parou de crescer: alloc final=%zu >= limite pedido=%zu, growth_failed=%s\n\n",
          buf.alloc, policy.limit, policy.growth_failed ? "true" : "false");

  free (buf.buf);

  puts ("=== Demonstracao 2: aritmetica de ponteiros (stack), begfield/limfield ===");
  puts ("(sort.c:1862-2010 -- ver 'Aritmetica de ponteiros' no CLAUDE.md)\n");

  char linebuf[] = "banana 42 2024-01-05";
  struct line line = { .text = linebuf, .length = strlen (linebuf) + 1 };
  struct keyfield key = { .sword = 1, .schar = 0, .eword = 1, .echar = 0,
                           .skipsblanks = false, .skipeblanks = false };

  printf ("linha de entrada: \"%s\"  (equivalente a rodar: sort -k2,2 -- 3 campos separados por espaco)\n",
          linebuf);
  printf ("endereco de 'line' (struct local em main): %p\n", (void *) &line);
  printf ("endereco de 'key'  (struct local em main): %p\n", (void *) &key);

  int marcador = 0;
  demo_extrair_campo (&line, &key, marcador);

  printf ("\ndiferenca de endereco entre o marcador em main (%p) e o marcador\n"
          "um nivel de pilha abaixo (ver saida acima) mostra o quadro de pilha\n"
          "sendo empilhado a cada chamada -- eh o 'consumo de stack' pedido no passo 11.\n",
          (void *) &marcador);

  return 0;
}
