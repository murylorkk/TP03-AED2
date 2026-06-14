#include <stdio.h>
#include <stdlib.h>
#include "TAD_arv_bin_pesquisa.h"

Arvore_bin* criar_arv_bin_pesquisa(){
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
        a = (Arvore_bin*) malloc(sizeof(Arvore_bin));
        a->chave = chave;
        a->rrn = rrn;
        a->esq = a->dir = NULL;
    }
    else if (chave < a->chave) {
        a->esq = inserir_arv_bin_pesquisa(a->esq, chave, rrn);
    }
    // IMPORTANTE: Se for igual (>=), joga para a direita para permitir repetições
    else if (chave >= a->chave) {
        a->dir = inserir_arv_bin_pesquisa(a->dir, chave, rrn);
    }
    return a;
}

int buscar_arv_bin_pesquisa(Arvore_bin* arv, int chave){
    if (arv == NULL){
        return -1; // Retorna -1 indicando que não achou o RRN
    }

    if (arv->chave == chave){
        return arv->rrn; // Retorna a coordenada do arquivo
    }

    if (chave < arv->chave){
        return buscar_arv_bin_pesquisa(arv->esq, chave);
    }
    else{
        return buscar_arv_bin_pesquisa(arv->dir, chave);
    }
}

void buscar_maior_que(Arvore_bin* arv, int valor_minimo, int* vetor_resultados, int* qtd) {
    if (arv != NULL) {
        // Se a chave atual é menor ou igual ao mínimo, 
        // os maiores só podem estar à direita. Ignoramos a esquerda.
        if (arv->chave <= valor_minimo) {
            buscar_maior_que(arv->dir, valor_minimo, vetor_resultados, qtd);
        } else {
            // Se a chave atual é MAIOR que o mínimo:
            // 1. Procura na esquerda (podem ter valores maiores que o min lá)
            buscar_maior_que(arv->esq, valor_minimo, vetor_resultados, qtd);
            
            // 2. Salva o RRN atual no vetor e incrementa o contador
            vetor_resultados[*qtd] = arv->rrn;
            (*qtd)++;
            
            // 3. Procura na direita (todos lá serão maiores)
            buscar_maior_que(arv->dir, valor_minimo, vetor_resultados, qtd);
        }
    }
}

int obter_altura_bin(Arvore_bin* no) {
    // ... (sua lógica original permanece idêntica)
    if (no == NULL){
        return 0;
    }
    int alt_esq = obter_altura_bin(no->esq);
    int alt_dir = obter_altura_bin(no->dir);
    int altura = 1;
    if(alt_esq > alt_dir){
        return altura + alt_esq;
    }
    else{
        return altura + alt_dir;
    }
}

void imprimir_em_ordem(Arvore_bin *arv){
    if (arv != NULL){
        imprimir_em_ordem(arv->esq);
        printf("<Chave: %d, RRN: %d> ", arv->chave, arv->rrn);
        imprimir_em_ordem(arv->dir);
    }
}

// Você pode atualizar pre_ordem e pos_ordem da mesma forma que imprimir_em_ordem
void imprimir_pre_ordem(Arvore_bin *arv){
    if (arv != NULL){
        printf("<Chave: %d, RRN: %d> ", arv->chave, arv->rrn);
        imprimir_pre_ordem(arv->esq);
        imprimir_pre_ordem(arv->dir);
    }
}

void imprimir_pos_ordem(Arvore_bin *arv){
    if (arv != NULL){
        imprimir_pos_ordem(arv->esq);
        imprimir_pos_ordem(arv->dir);
        printf("<Chave: %d, RRN: %d> ", arv->chave, arv->rrn);
    }
}