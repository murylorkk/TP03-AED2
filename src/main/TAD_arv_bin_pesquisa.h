#ifndef ARVORE_H
#define ARVORE_H

struct arvore_bin {
    int chave;  // atributo que está sendo indexado
    int rrn;    // posição do registro no arquivo (fseek)
    struct arvore_bin* esq;
    struct arvore_bin* dir;
};

typedef struct arvore_bin Arvore_bin;

Arvore_bin* criar_arv_bin_pesquisa();

void destruir_arv_bin_pesquisa(Arvore_bin* a);

Arvore_bin* inserir_arv_bin_pesquisa(Arvore_bin* a, int chave, int rrn);

int buscar_arv_bin_pesquisa(Arvore_bin* a, int chave);

void buscar_maior_que(Arvore_bin* a, int valor_minimo, int* vetor_resultados, int* qtd_encontrados);

int obter_altura_bin(Arvore_bin* no);

void imprimir_pre_ordem(Arvore_bin* a);

void imprimir_em_ordem(Arvore_bin* a);

void imprimir_pos_ordem(Arvore_bin* a);

#endif