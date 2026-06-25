#include <stdio.h>
#include <stdlib.h>
#include "TAD_hash.h"

static int funcao_hash(int chave) {
    return chave % TAM_HASH;
}

void hash_inicializar(Hash *h) {
    for (int i = 0; i < TAM_HASH; i++) {
        h->tabela[i] = NULL;
    }

    h->colisoes = 0;
}

void hash_inserir(Hash *h, int chave, int rrn) {

    int pos = funcao_hash(chave);

    if (h->tabela[pos] != NULL) {
        h->colisoes++;
    }

    NoHash *novo = (NoHash *) malloc(sizeof(NoHash));

    if (novo == NULL) {
        printf("Erro de memoria.\n");
        exit(1);
    }

    novo->chave = chave;
    novo->rrn = rrn;

    novo->prox = h->tabela[pos];
    h->tabela[pos] = novo;
}

int hash_buscar(Hash *h, int chave) {

    int pos = funcao_hash(chave);

    NoHash *aux = h->tabela[pos];

    while (aux != NULL) {

        if (aux->chave == chave) {
            return aux->rrn;
        }

        aux = aux->prox;
    }

    return -1;
}

void hash_mostrar(Hash *h) {

    for (int i = 0; i < TAM_HASH; i++) {

        NoHash *aux = h->tabela[i];

        if (aux != NULL) {

            printf("[%d] -> ", i);

            while (aux != NULL) {
                printf("<%d,%d> ", aux->chave, aux->rrn);
                aux = aux->prox;
            }

            printf("\n");
        }
    }
}

void hash_destruir(Hash *h) {

    for (int i = 0; i < TAM_HASH; i++) {

        NoHash *aux = h->tabela[i];

        while (aux != NULL) {

            NoHash *temp = aux;
            aux = aux->prox;

            free(temp);
        }

        h->tabela[i] = NULL;
    }
}
