/*
 * mymemory.c - gerenciador de memoria com pool pre-alocado (Trabalho 2 - PSB)
 *
 * Ideia geral:
 *   - um unico malloc grande (o pool); toda memoria entregue ao usuario
 *     sai de dentro dele;
 *   - uma lista encadeada de allocation_t guarda so as regioes OCUPADAS,
 *     sempre ordenada por endereco (start crescente);
 *   - o espaco livre e o que sobra entre um no e o proximo.
 *
 * Estrategia de alocacao: First Fit (primeiro buraco onde o pedido cabe).
 *
 * Contas de endereco sao feitas com char *, porque char tem 1 byte:
 * somar n em um char * anda exatamente n bytes.
 */

#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

/* Todo tamanho pedido e arredondado para multiplo de 8, para que um int*,
 * double* etc. nunca caia em endereco desalinhado dentro do pool. */
#define ALINHAMENTO 8

mymemory_t* mymemory_init(size_t size)
{
    mymemory_t *memory = malloc(sizeof(mymemory_t));
    if (memory == NULL)
        return NULL;

    memory->pool = malloc(size);
    if (memory->pool == NULL) {
        free(memory);
        return NULL;
    }

    memory->total_size = size;
    memory->head = NULL;        /* lista vazia: nada alocado ainda */
    return memory;
}

void* mymemory_alloc(mymemory_t *memory, size_t size)
{
    if (memory == NULL || size == 0)
        return NULL;

    size = (size + ALINHAMENTO - 1) / ALINHAMENTO * ALINHAMENTO;

    char *fim_pool = (char *)memory->pool + memory->total_size;

    /* Percorre os buracos livres em ordem de endereco.
     * "livre" marca onde comeca o buraco atual. */
    char *livre = (char *)memory->pool;
    allocation_t *anterior = NULL;
    allocation_t *atual = memory->head;

    while (atual != NULL) {
        char *inicio_ocupado = (char *)atual->start;

        /* buraco entre "livre" e o inicio do no atual */
        if ((size_t)(inicio_ocupado - livre) >= size)
            break;                          /* cabe aqui: First Fit */

        livre = inicio_ocupado + atual->size;  /* pula a regiao ocupada */
        anterior = atual;
        atual = atual->next;
    }

    /* Saiu do laco sem achar buraco: resta o espaco depois do ultimo no */
    if (atual == NULL && (size_t)(fim_pool - livre) < size)
        return NULL;

    allocation_t *novo = malloc(sizeof(allocation_t));
    if (novo == NULL)
        return NULL;
    novo->start = livre;
    novo->size = size;

    /* Insere entre "anterior" e "atual", mantendo a lista ordenada */
    novo->next = atual;
    if (anterior == NULL)
        memory->head = novo;                /* virou o primeiro da lista */
    else
        anterior->next = novo;

    return novo->start;
}

void mymemory_free(mymemory_t *memory, void *ptr)
{
    if (memory == NULL || ptr == NULL)
        return;

    allocation_t *anterior = NULL;
    allocation_t *atual = memory->head;

    while (atual != NULL && atual->start != ptr) {
        anterior = atual;
        atual = atual->next;
    }

    if (atual == NULL)
        return;                             /* ptr invalido: nao faz nada */

    if (anterior == NULL)
        memory->head = atual->next;
    else
        anterior->next = atual->next;

    free(atual);    /* libera o NO; a regiao do pool volta a ser buraco */
}

void mymemory_display(mymemory_t *memory)
{
    if (memory == NULL)
        return;

    printf("--- Alocacoes (pool de %zu bytes em %p) ---\n",
           memory->total_size, memory->pool);

    if (memory->head == NULL) {
        printf("(nenhuma alocacao)\n");
        return;
    }

    allocation_t *atual = memory->head;
    int i = 1;
    while (atual != NULL) {
        size_t posicao = (size_t)((char *)atual->start - (char *)memory->pool);
        printf("%2d) inicio %p | bytes %zu a %zu do pool | tamanho %zu\n",
               i, atual->start, posicao, posicao + atual->size - 1, atual->size);
        atual = atual->next;
        i++;
    }
}

void mymemory_stats(mymemory_t *memory)
{
    if (memory == NULL)
        return;

    size_t num_alocacoes = 0;
    size_t total_alocado = 0;
    size_t maior_livre = 0;
    size_t fragmentos = 0;

    char *fim_pool = (char *)memory->pool + memory->total_size;
    char *livre = (char *)memory->pool;
    allocation_t *atual = memory->head;

    while (atual != NULL) {
        size_t buraco = (size_t)((char *)atual->start - livre);
        if (buraco > 0) {
            fragmentos++;
            if (buraco > maior_livre)
                maior_livre = buraco;
        }

        num_alocacoes++;
        total_alocado += atual->size;

        livre = (char *)atual->start + atual->size;
        atual = atual->next;
    }

    /* buraco final, depois do ultimo no */
    size_t buraco = (size_t)(fim_pool - livre);
    if (buraco > 0) {
        fragmentos++;
        if (buraco > maior_livre)
            maior_livre = buraco;
    }

    printf("--- Estatisticas ---\n");
    printf("Numero de alocacoes:        %zu\n", num_alocacoes);
    printf("Memoria alocada:            %zu bytes\n", total_alocado);
    printf("Memoria livre:              %zu bytes\n",
           memory->total_size - total_alocado);
    printf("Maior bloco livre contiguo: %zu bytes\n", maior_livre);
    printf("Fragmentos livres:          %zu\n", fragmentos);
}

void mymemory_cleanup(mymemory_t *memory)
{
    if (memory == NULL)
        return;

    allocation_t *atual = memory->head;
    while (atual != NULL) {
        allocation_t *proximo = atual->next;   /* guarda antes do free */
        free(atual);
        atual = proximo;
    }

    free(memory->pool);
    free(memory);
}
