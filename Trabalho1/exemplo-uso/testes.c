/* testes.c
 *
 * Testes automatizados do subconjunto estudado de sort.c (passo 12 do
 * roteiro, CLAUDE.md -- "Construcao e testes automatizados"). O
 * enunciado nao fixa uma quantidade minima de testes; estabelecemos a
 * nossa: pelo menos 1 teste de caminho normal + 1 de caso de borda pra
 * cada uma das 2 duplas de funcoes do subconjunto que tem logica
 * condicional (buffer_linelim/line_aligned_size/try_growbuf/
 * maybe_growbuf, e begfield/limfield) -- 10 testes no total, cada um
 * checando um comportamento especifico e citando a linha de sort.c que
 * ele exercita.
 *
 * Sem framework externo (xunit, Check, etc.) -- so uma funcao check()
 * com contador de passa/falha e saida com codigo de erro != 0 se algum
 * teste falhar, pra dar pra usar em "make test" / CI sem olho humano.
 *
 * Compilar:  gcc -O0 -g -Wall -Wextra -std=gnu17 -o testes testes.c
 * Rodar:     ./testes   (imprime cada teste, sai com status 1 se falhar)
 */

#include "subconjunto.h"

static int total = 0;
static int falhas = 0;

static void
check (char const *nome, bool condicao, char const *detalhe)
{
  total++;
  if (condicao)
    printf ("  OK   %s\n", nome);
  else
    {
      falhas++;
      printf ("  FAIL %s -- %s\n", nome, detalhe);
    }
}

/* -----------------------------------------------------------------------
 * Grupo 1: buffer_linelim / line_aligned_size / try_growbuf / maybe_growbuf
 * (heap) -- sort.c:1791-1857
 * --------------------------------------------------------------------- */

static void
teste_crescimento_normal (void)
{
  struct buffer buf = { .buf = malloc (64), .alloc = 64 };
  struct sort_buffer_policy policy = { .limit = 5000 };

  bool cresceu = maybe_growbuf (&buf, &policy);
  check ("maybe_growbuf cresce quando ha espaco (limit >> alloc)",
         cresceu && buf.alloc > 64,
         "esperava crescer alem de 64 bytes na primeira chamada");

  size_t alloc_apos_1a_chamada = buf.alloc;
  while (maybe_growbuf (&buf, &policy))
    ;
  check ("maybe_growbuf converge: alloc final >= limite pedido",
         buf.alloc >= policy.limit,
         "esperava alloc final >= 5000 apos crescimento repetido");
  check ("maybe_growbuf converge sem marcar growth_failed",
         ! policy.growth_failed,
         "growth_failed deveria continuar false (parou pelo limite, nao por malloc falhar)");
  (void) alloc_apos_1a_chamada;

  free (buf.buf);
}

static void
teste_crescimento_bloqueado_por_limite (void)
{
  /* sort.c:1843 -- "policy->limit <= buf->alloc" deve retornar false
     imediatamente, sem tentar realocar. */
  struct buffer buf = { .buf = malloc (1000), .alloc = 1000 };
  struct sort_buffer_policy policy = { .limit = 500 };
  char *ptr_antes = buf.buf;

  bool cresceu = maybe_growbuf (&buf, &policy);
  check ("maybe_growbuf nao cresce quando limite <= alloc atual",
         ! cresceu && buf.alloc == 1000 && buf.buf == ptr_antes,
         "esperava false, alloc/ponteiro inalterados (limit=500 <= alloc=1000)");

  free (buf.buf);
}

static void
teste_maybe_growbuf_sem_policy (void)
{
  /* sort.c:1843 -- "! policy" deve ser tratado com seguranca (guarda
     defensiva contra ponteiro nulo). */
  struct buffer buf = { .buf = malloc (64), .alloc = 64 };
  bool cresceu = maybe_growbuf (&buf, NULL);
  check ("maybe_growbuf com policy=NULL nao cresce nem falha",
         ! cresceu,
         "esperava false com policy nulo, sem crash");
  free (buf.buf);
}

static void
teste_try_growbuf_alloc_menor_ou_igual (void)
{
  /* sort.c:1805 -- "alloc <= buf->alloc" deve recusar diminuir/manter. */
  struct buffer buf = { .buf = malloc (256), .alloc = 256 };
  bool cresceu = try_growbuf (&buf, 256);
  check ("try_growbuf recusa alloc igual ao atual",
         ! cresceu && buf.alloc == 256,
         "esperava false quando alloc pedido == alloc atual");
  free (buf.buf);
}

static void
teste_heap_realoca_endereco_novo (void)
{
  /* Confirma que o crescimento eh malloc-novo-bloco + free-do-antigo
     (sort.c:1808-1834), nao um realloc in-place. */
  struct buffer buf = { .buf = malloc (64), .alloc = 64 };
  struct sort_buffer_policy policy = { .limit = 5000 };
  char *ptr_antes = buf.buf;

  bool cresceu = maybe_growbuf (&buf, &policy);
  check ("apos crescer, o endereco do buffer muda (malloc novo, nao in-place)",
         cresceu && buf.buf != ptr_antes,
         "esperava um ponteiro de heap diferente apos o crescimento");

  free (buf.buf);
}

/* -----------------------------------------------------------------------
 * Grupo 2: begfield / limfield (aritmetica de ponteiros) -- sort.c:1862-2010
 * --------------------------------------------------------------------- */

static struct line
linha_de (char *texto)
{
  return (struct line) { .text = texto, .length = strlen (texto) + 1 };
}

static void
teste_campo_com_separador_incluido (void)
{
  /* sort.c:1869-1871 -- "leading field separator is included ... when
     -t is absent". Regressao do achado do passo 11 (saida-exemplo.txt). */
  char texto[] = "banana 42 2024-01-05";
  struct line line = linha_de (texto);
  struct keyfield key = { .sword = 1, .eword = 1, .echar = 0 }; /* -k2,2, sem -b */

  char *inicio = begfield (&line, &key);
  char *fim = limfield (&line, &key);

  check ("-k2,2 sem -b inclui o separador: campo == \" 42\"",
         (fim - inicio) == 3 && memcmp (inicio, " 42", 3) == 0,
         "esperava begfield=offset 6, limfield=offset 9, texto \" 42\"");
}

static void
teste_campo_com_skipsblanks (void)
{
  /* sort.c:1891-1893 -- skipsblanks pula o separador (equivalente a -b). */
  char texto[] = "banana 42 2024-01-05";
  struct line line = linha_de (texto);
  struct keyfield key = { .sword = 1, .eword = 1, .echar = 0, .skipsblanks = true };

  char *inicio = begfield (&line, &key);
  char *fim = limfield (&line, &key);

  check ("-k2,2 com -b (skipsblanks) da campo limpo == \"42\"",
         (fim - inicio) == 2 && memcmp (inicio, "42", 2) == 0,
         "esperava begfield=offset 7, limfield=offset 9, texto \"42\"");
}

static void
teste_primeiro_campo (void)
{
  /* sword=0: o while externo nunca entra (sword-- comeca falso), entao
     begfield deve retornar o proprio inicio da linha sem andar. */
  char texto[] = "banana 42 2024-01-05";
  struct line line = linha_de (texto);
  struct keyfield key = { .sword = 0, .schar = 0 };

  char *inicio = begfield (&line, &key);
  check ("sword=0 retorna o inicio da linha sem avancar",
         inicio == line.text,
         "esperava begfield == line.text quando sword=0");
}

static void
teste_campo_alem_do_fim (void)
{
  /* sword maior que o numero de campos existentes: begfield deve parar
     em lim (fim da linha), nunca ler alem do buffer. */
  char texto[] = "banana 42 2024-01-05"; /* so 3 campos (indices 0-2) */
  struct line line = linha_de (texto);
  struct keyfield key = { .sword = 5, .schar = 0 };
  char *lim_esperado = line.text + line.length - 1;

  char *inicio = begfield (&line, &key);
  check ("sword alem do numero de campos para em lim (sem estourar o buffer)",
         inicio == lim_esperado,
         "esperava begfield == text+length-1 (fim da linha) quando sword > campos existentes");
}

static void
teste_campo_delimitado_por_tab (void)
{
  /* sort.c:1872-1879 -- ramo memchr, usado quando -t define um
     delimitador diferente do espaco em branco. */
  char texto[] = "a\tbb\tccc";
  struct line line = linha_de (texto);
  struct keyfield key = { .sword = 1, .schar = 0 };

  int tab_anterior = tab;
  tab = '\t'; /* simula -t $'\t' */
  char *inicio = begfield (&line, &key);
  tab = tab_anterior; /* restaura o global pra nao vazar estado pros outros testes */

  check ("com -t (tab != TAB_DEFAULT), begfield usa memchr e acha o 2o campo",
         inicio == line.text + 2,
         "esperava begfield apontar pra \"bb\" (offset 2) com delimitador tab");
}

int
main (void)
{
  init_blanks ();

  puts ("=== Grupo 1: buffer (heap) -- try_growbuf/maybe_growbuf ===");
  teste_crescimento_normal ();
  teste_crescimento_bloqueado_por_limite ();
  teste_maybe_growbuf_sem_policy ();
  teste_try_growbuf_alloc_menor_ou_igual ();
  teste_heap_realoca_endereco_novo ();

  puts ("\n=== Grupo 2: aritmetica de ponteiros -- begfield/limfield ===");
  teste_campo_com_separador_incluido ();
  teste_campo_com_skipsblanks ();
  teste_primeiro_campo ();
  teste_campo_alem_do_fim ();
  teste_campo_delimitado_por_tab ();

  printf ("\n%d/%d testes passaram.\n", total - falhas, total);
  return falhas == 0 ? 0 : 1;
}
