/* harness_subconjunto.c
 *
 * Exemplo de uso do subconjunto estudado de sort.c (passo 11 do roteiro,
 * CLAUDE.md). As funcoes de sort.c (buffer_linelim, line_aligned_size,
 * try_growbuf, maybe_growbuf, begfield, limfield) e os stubs de suporte
 * estao em subconjunto.h -- este arquivo so tem a demonstracao (main).
 * Testes automatizados de verdade estao em testes.c (passo 12).
 *
 * Compilar:  gcc -O0 -g -Wall -Wextra -std=gnu17 -o demo harness_subconjunto.c
 * Rodar:     ./demo
 * (ou "make" / "make run" -- ver Makefile)
 */

#include "subconjunto.h"

/* Chama begfield()/limfield() um nivel de pilha abaixo de main(), so pra
 * deixar visivel no print a diferenca de endereco entre os dois quadros
 * (stack cresce "pra baixo" em x86-64: enderecos menores = mais fundo
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
