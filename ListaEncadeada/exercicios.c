#include <stdio.h>
#include <stdlib.h>

#define TAM 6

typedef struct Node {
    int value;
    struct Node *next;
} Node;

int buscar(int *v, int n, int x)
{
    for (int i = 0; i < n; i++) {
        if (*(v + i) == x)
            return i;
    }
    return -1;
}

void copiar(int *origem, int *destino, int n)
{
    for (int i = 0; i < n; i++)
        *(destino + i) = *(origem + i);
}

void inverter(int *v, int n)
{
    int *ini = v;
    int *fim = v + n - 1;

    while (ini < fim) {
        int aux = *ini;
        *ini = *fim;
        *fim = aux;
        ini++;
        fim--;
    }
}

void ordenar(int *v, int n)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (*(v + j) > *(v + j + 1)) {
                int aux = *(v + j);
                *(v + j) = *(v + j + 1);
                *(v + j + 1) = aux;
            }
        }
    }
}

void mostrarVetor(int *v, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", *(v + i));
    printf("\n");
}

void testarVetores(void)
{
    int v[TAM] = {5, 3, 8, 1, 9, 2};
    int copia[TAM];

    printf("\nVetor original: ");
    mostrarVetor(v, TAM);

    printf("\n1) Buscar\n");
    printf("Posicao do 8: %d\n", buscar(v, TAM, 8));
    printf("Posicao do 7: %d\n", buscar(v, TAM, 7));

    printf("\n2) Copiar\n");
    copiar(v, copia, TAM);
    printf("Copia: ");
    mostrarVetor(copia, TAM);

    printf("\n3) Inverter o original\n");
    inverter(v, TAM);
    printf("Original invertido: ");
    mostrarVetor(v, TAM);

    printf("\n4) Ordenar a copia\n");
    ordenar(copia, TAM);
    printf("Copia ordenada: ");
    mostrarVetor(copia, TAM);
}

void add(Node **head, int value)
{
    Node *newNode = malloc(sizeof *newNode);
    if (newNode == NULL) {
        printf("Sem memoria\n");
        return;
    }
    newNode->value = value;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Node *current = *head;
    while (current->next != NULL)
        current = current->next;
    current->next = newNode;
}

void addStart(Node **head, int value)
{
    Node *newNode = malloc(sizeof *newNode);
    if (newNode == NULL) {
        printf("Sem memoria\n");
        return;
    }
    newNode->value = value;
    newNode->next = *head;
    *head = newNode;
}

int removeByValue(Node **head, int value)
{
    Node *current = *head;
    Node *previous = NULL;

    while (current != NULL && current->value != value) {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
        return 0;

    if (previous == NULL)
        *head = current->next;
    else
        previous->next = current->next;

    free(current);
    return 1;
}

void show(Node *head)
{
    if (head == NULL) {
        printf("Lista vazia\n");
        return;
    }

    for (Node *current = head; current != NULL; current = current->next)
        printf("%d -> ", current->value);
    printf("NULL\n");
}

void freeList(Node **head)
{
    Node *current = *head;

    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

void menuLista(void)
{
    Node *head = NULL;
    int opcao = -1;
    int valor = 0;

    while (opcao != 0) {
        printf("\n=== LISTA ENCADEADA ===\n");
        printf("1 - Inserir no final (add)\n");
        printf("2 - Inserir no inicio (addStart)\n");
        printf("3 - Remover por valor (removeByValue)\n");
        printf("4 - Mostrar lista (show)\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1)
            opcao = 0;

        switch (opcao) {
        case 1:
            printf("Valor: ");
            scanf("%d", &valor);
            add(&head, valor);
            break;
        case 2:
            printf("Valor: ");
            scanf("%d", &valor);
            addStart(&head, valor);
            break;
        case 3:
            printf("Valor: ");
            scanf("%d", &valor);
            if (removeByValue(&head, valor))
                printf("Valor %d removido\n", valor);
            else
                printf("Valor %d nao encontrado\n", valor);
            break;
        case 4:
            show(head);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida\n");
        }
    }

    freeList(&head);
}

int main(void)
{
    int opcao = -1;

    while (opcao != 0) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Testar vetores\n");
        printf("2 - Lista encadeada\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1)
            opcao = 0;

        switch (opcao) {
        case 1:
            testarVetores();
            break;
        case 2:
            menuLista();
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida\n");
        }
    }

    return 0;
}
