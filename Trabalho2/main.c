/*
 * main.c - testes do gerenciador de memoria (Trabalho 2 - PSB)
 *
 * Parte 1: cenario do main.c da professora (com os nomes do mymemory.h).
 * Parte 2: casos especiais (tamanho 0, maior que o pool, free invalido).
 * Parte 3: ~50 blocos de tamanhos variados em 3 ordens diferentes.
 *
 * Cada verificacao conta acerto/erro; no fim o programa retorna 1 se algo
 * falhou, para o teste poder ser automatizado.
 */

#include <stdio.h>
#include <string.h>
#include "mymemory.h"

#define NUM_BLOCOS 50
#define TAM_POOL 10000

int acertos = 0;
int erros = 0;

void verifica(int condicao, const char *descricao)
{
    if (condicao) {
        acertos++;
    } else {
        erros++;
        printf("FALHOU: %s\n", descricao);
    }
}

/* Confere a lista inteira: cada bloco dentro do pool, em ordem de
 * endereco, sem sobrepor o anterior. */
void verifica_lista(mymemory_t *memory)
{
    char *inicio_pool = (char *)memory->pool;
    char *fim_pool = inicio_pool + memory->total_size;
    char *fim_anterior = inicio_pool;
    allocation_t *atual = memory->head;
    int ok = 1;

    while (atual != NULL) {
        char *inicio = (char *)atual->start;
        char *fim = inicio + atual->size;
        if (inicio < fim_anterior || fim > fim_pool)
            ok = 0;
        fim_anterior = fim;
        atual = atual->next;
    }
    verifica(ok, "blocos dentro do pool, ordenados e sem sobreposicao");
}

void parte1_exemplo_professora(void)
{
    printf("\n===== Parte 1: exemplo da professora =====\n");

    mymemory_t *memory = mymemory_init(1000);
    verifica(memory != NULL, "init do pool de 1000 bytes");

    int *v = mymemory_alloc(memory, sizeof(int) * 32);
    verifica(v != NULL, "alocar 32 int");
    for (int i = 0; i < 32; i++)
        v[i] = i + 1;

    int *v2 = mymemory_alloc(memory, sizeof(int) * 16);
    verifica(v2 != NULL, "alocar 16 int");
    for (int i = 0; i < 16; i++)
        v2[i] = 100 + i;

    printf("\nDepois de alocar v (128 bytes) e v2 (64 bytes):\n");
    mymemory_display(memory);

    mymemory_free(memory, v);

    char *str = mymemory_alloc(memory, sizeof(char) * 16);
    verifica(str != NULL, "alocar 16 char");
    sprintf(str, "ola mundo");
    verifica((void *)str == (void *)v, "First Fit reaproveita o buraco deixado por v");
    verifica(v2[15] == 115, "v2 continua intacto");

    printf("\nDepois de free(v) e alocar str (16 bytes) -> \"%s\":\n", str);
    mymemory_display(memory);
    mymemory_stats(memory);
    verifica_lista(memory);

    mymemory_cleanup(memory);
}

void parte2_casos_especiais(void)
{
    printf("\n===== Parte 2: casos especiais =====\n");

    mymemory_t *memory = mymemory_init(100);
    int numero_local = 0;

    verifica(mymemory_alloc(memory, 0) == NULL, "alocar 0 bytes retorna NULL");
    verifica(mymemory_alloc(memory, 101) == NULL, "alocar mais que o pool retorna NULL");

    char *a = mymemory_alloc(memory, 96);
    verifica(a != NULL, "alocar 96 bytes (cabe exato no pool alinhado)");
    verifica(mymemory_alloc(memory, 1) == NULL, "pool cheio retorna NULL");

    mymemory_free(memory, &numero_local);   /* ponteiro que nao e do pool */
    mymemory_free(memory, NULL);
    verifica(memory->head != NULL, "free de ponteiro invalido nao faz nada");

    mymemory_free(memory, a);
    mymemory_free(memory, a);               /* segundo free do mesmo ponteiro */
    verifica(memory->head == NULL, "free duplo nao quebra a lista");

    printf("Casos especiais executados.\n");
    mymemory_cleanup(memory);
}

/* Aloca NUM_BLOCOS blocos na ordem dada, libera metade (intercalados)
 * e tenta alocar de novo, mostrando como a fragmentacao muda. */
void testa_ordem(const char *nome, size_t tamanhos[])
{
    void *blocos[NUM_BLOCOS];
    int alocados = 0;

    printf("\n----- Ordem %s -----\n", nome);
    mymemory_t *memory = mymemory_init(TAM_POOL);

    for (int i = 0; i < NUM_BLOCOS; i++) {
        blocos[i] = mymemory_alloc(memory, tamanhos[i]);
        if (blocos[i] != NULL) {
            memset(blocos[i], 'x', tamanhos[i]);    /* usa o bloco de verdade */
            alocados++;
        }
    }
    printf("Alocados %d de %d blocos.\n", alocados, NUM_BLOCOS);
    mymemory_stats(memory);
    verifica_lista(memory);

    /* libera os blocos de indice par: cria buracos intercalados */
    for (int i = 0; i < NUM_BLOCOS; i += 2)
        mymemory_free(memory, blocos[i]);
    printf("\nDepois de liberar os blocos de indice par:\n");
    mymemory_stats(memory);
    verifica_lista(memory);

    /* tenta reaproveitar os buracos com blocos de 200 bytes */
    int realocados = 0;
    for (int i = 0; i < 20; i++)
        if (mymemory_alloc(memory, 200) != NULL)
            realocados++;
    printf("\nNovos blocos de 200 bytes que couberam nos buracos: %d de 20\n", realocados);
    mymemory_stats(memory);
    verifica_lista(memory);

    mymemory_cleanup(memory);
}

void parte3_cinquenta_blocos(void)
{
    printf("\n===== Parte 3: %d blocos, pool de %d bytes =====\n", NUM_BLOCOS, TAM_POOL);

    size_t base[7] = {10, 25, 50, 100, 250, 500, 1000};
    size_t crescente[NUM_BLOCOS];
    size_t decrescente[NUM_BLOCOS];
    size_t misturada[NUM_BLOCOS];

    for (int i = 0; i < NUM_BLOCOS; i++) {
        crescente[i] = base[i * 7 / NUM_BLOCOS];        /* 10,10,...,1000 */
        decrescente[i] = base[6 - i * 7 / NUM_BLOCOS];  /* 1000,...,10 */
        misturada[i] = base[i % 7];                     /* 10,25,...,1000,10,... */
    }

    testa_ordem("crescente", crescente);
    testa_ordem("decrescente", decrescente);
    testa_ordem("misturada", misturada);
}

int main(void)
{
    parte1_exemplo_professora();
    parte2_casos_especiais();
    parte3_cinquenta_blocos();

    printf("\n===== Resultado: %d verificacoes OK, %d falhas =====\n", acertos, erros);
    if (erros > 0)
        return 1;
    return 0;
}
