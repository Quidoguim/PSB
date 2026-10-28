#include <stdio.h>
#include <stdlib.h>

typedef struct TNode {
    int data;
    struct TNode* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} List;

void add(List* l, int val);
void addStart(List* l, int val);
int removeByValue(List* l, int val);
void show(List* l);
void freeList(List* l);

void add(List* l, int val)
{
    Node* aux = malloc(sizeof(Node));
    aux->data = val;
    aux->next = NULL;

    if(l->tail != NULL)
        l->tail->next = aux;
    else
        l->head = aux;
    l->tail = aux;
    l->size++;
}

void addStart(List* l, int val)
{
    Node* aux = malloc(sizeof(Node));
    aux->data = val;
    aux->next = l->head;

    l->head = aux;
    if(l->tail == NULL)
        l->tail = aux;
    l->size++;
}

int removeByValue(List* l, int val)
{
    Node* atual = l->head;
    Node* anterior = NULL;

    while(atual != NULL && atual->data != val) {
        anterior = atual;
        atual = atual->next;
    }

    if(atual == NULL)
        return 0;

    if(anterior == NULL)
        l->head = atual->next;
    else
        anterior->next = atual->next;

    if(atual == l->tail)
        l->tail = anterior;

    free(atual);
    l->size--;
    return 1;
}

void show(List* l)
{
    Node* atual = l->head;

    if(atual == NULL) {
        printf("Lista vazia\n");
        return;
    }

    while(atual != NULL) {
        printf("%d ", atual->data);
        atual = atual->next;
    }
    printf("\n");
}

void freeList(List* l)
{
    Node* atual = l->head;

    while(atual != NULL) {
        Node* prox = atual->next;
        free(atual);
        atual = prox;
    }
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

int main(void)
{
    List lista;
    lista.head = lista.tail = NULL;
    lista.size = 0;

    int opcao = -1;
    int val;

    while(opcao != 0) {
        printf("\n1 - Inserir no final\n");
        printf("2 - Inserir no inicio\n");
        printf("3 - Remover por valor\n");
        printf("4 - Mostrar lista\n");
        printf("5 - Liberar lista\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if(opcao == 1) {
            printf("Valor: ");
            scanf("%d", &val);
            add(&lista, val);
        }
        else if(opcao == 2) {
            printf("Valor: ");
            scanf("%d", &val);
            addStart(&lista, val);
        }
        else if(opcao == 3) {
            printf("Valor: ");
            scanf("%d", &val);
            if(removeByValue(&lista, val))
                printf("Valor removido\n");
            else
                printf("Valor nao encontrado\n");
        }
        else if(opcao == 4) {
            show(&lista);
        }
        else if(opcao == 5) {
            freeList(&lista);
            printf("Lista liberada\n");
        }
        else if(opcao != 0) {
            printf("Opcao invalida\n");
        }
    }

    freeList(&lista);
    return 0;
}
