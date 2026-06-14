#include <stdio.h>
#include <stdlib.h>
#include "TAD_arv_bin_pesquisa.h"

Arvore_bin* criar_arv_bin_pesquisa() {
    return NULL;
}

void destruir_arv_bin_pesquisa(Arvore_bin* a) {
    if (a != NULL) {
        destruir_arv_bin_pesquisa(a->esq);
        destruir_arv_bin_pesquisa(a->dir);
        free(a);
    }
}

Arvore_bin* inserir_arv_bin_pesquisa(Arvore_bin* a, int chave, int rrn) {
    if (a == NULL) {
        Arvore_bin* novo = (Arvore_bin*) malloc(sizeof(Arvore_bin));

        if (novo == NULL) {
            printf("Erro de memoria.\n");
            exit(1);
        }

        novo->chave = chave;
        novo->rrn = rrn;
        novo->esq = NULL;
        novo->dir = NULL;

        return novo;
    }

    if (chave < a->chave) {
        a->esq = inserir_arv_bin_pesquisa(a->esq, chave, rrn);
    } else {
        a->dir = inserir_arv_bin_pesquisa(a->dir, chave, rrn);
    }

    return a;
}

int buscar_arv_bin_pesquisa(Arvore_bin* arv, int chave) {
    if (arv == NULL) {
        return -1;
    }

    if (arv->chave == chave) {
        return arv->rrn;
    }

    if (chave < arv->chave) {
        return buscar_arv_bin_pesquisa(arv->esq, chave);
    }

    return buscar_arv_bin_pesquisa(arv->dir, chave);
}

void buscar_maior_que(Arvore_bin* arv, int valor_minimo, int* vetor_resultados, int* qtd) {
    if (arv == NULL) {
        return;
    }

    if (arv->chave <= valor_minimo) {
        buscar_maior_que(arv->dir, valor_minimo, vetor_resultados, qtd);
    } else {
        buscar_maior_que(arv->esq, valor_minimo, vetor_resultados, qtd);

        vetor_resultados[*qtd] = arv->rrn;
        (*qtd)++;

        buscar_maior_que(arv->dir, valor_minimo, vetor_resultados, qtd);
    }
}

int obter_altura_bin(Arvore_bin* no) {
    if (no == NULL) {
        return 0;
    }

    int alt_esq = obter_altura_bin(no->esq);
    int alt_dir = obter_altura_bin(no->dir);

    if (alt_esq > alt_dir) {
        return 1 + alt_esq;
    }

    return 1 + alt_dir;
}

void imprimir_em_ordem(Arvore_bin* arv) {
    if (arv != NULL) {
        imprimir_em_ordem(arv->esq);
        printf("<Chave: %d, RRN: %d> ", arv->chave, arv->rrn);
        imprimir_em_ordem(arv->dir);
    }
}

void imprimir_pre_ordem(Arvore_bin* arv) {
    if (arv != NULL) {
        printf("<Chave: %d, RRN: %d> ", arv->chave, arv->rrn);
        imprimir_pre_ordem(arv->esq);
        imprimir_pre_ordem(arv->dir);
    }
}

void imprimir_pos_ordem(Arvore_bin* arv) {
    if (arv != NULL) {
        imprimir_pos_ordem(arv->esq);
        imprimir_pos_ordem(arv->dir);
        printf("<Chave: %d, RRN: %d> ", arv->chave, arv->rrn);
    }
}
