#include "TAD_hash.h"
#include <stdlib.h>

static int funcao_hash(int chave, int tamanho) {
    return abs(chave) % tamanho;
}

HashTable* criar_tabela(int tamanho) {
    if (tamanho <= 0) return NULL;

    HashTable* tabela = (HashTable*)malloc(sizeof(HashTable));
    if (!tabela) return NULL;

    tabela->vetor = (HashNode**)calloc(tamanho, sizeof(HashNode*));
    if (!tabela->vetor) {
        free(tabela);
        return NULL;
    }

    tabela->tamanho = tamanho;
    tabela->colisoes_totais = 0;

    return tabela;
}

int inserir_hash(HashTable* tabela, int chave, long numero_registro) {
    if (!tabela) return 0;

    int indice = funcao_hash(chave, tabela->tamanho);
    HashNode* atual = tabela->vetor[indice];

    if (atual != NULL) {
        tabela->colisoes_totais++;
    }

    while (atual != NULL) {
        if (atual->chave == chave) {
            atual->numero_registro = numero_registro;
            return 1;
        }
        atual = atual->prox;
    }

    HashNode* novo_no = (HashNode*)malloc(sizeof(HashNode));
    if (!novo_no) return 0;

    novo_no->chave = chave;
    novo_no->numero_registro = numero_registro;
    novo_no->prox = tabela->vetor[indice];
    tabela->vetor[indice] = novo_no;

    return 1;
}

long buscar_hash(HashTable* tabela, int chave, int* colisoes_busca) {
    if (!tabela || !colisoes_busca) return -1;

    *colisoes_busca = 0;
    int indice = funcao_hash(chave, tabela->tamanho);
    HashNode* atual = tabela->vetor[indice];

    while (atual != NULL) {
        if (atual->chave == chave) {
            return atual->numero_registro;
        }
        (*colisoes_busca)++;
        atual = atual->prox;
    }

    return -1;
}

void liberar_tabela(HashTable* tabela) {
    if (!tabela) return;

    for (int i = 0; i < tabela->tamanho; i++) {
        HashNode* atual = tabela->vetor[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }

    free(tabela->vetor);
    free(tabela);
}