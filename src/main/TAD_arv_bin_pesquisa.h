#ifndef ARVORE_H
#define ARVORE_H

struct arvore_bin {
    int chave;  // O atributo que está sendo indexado
    int rrn;    // A posição do registro no arquivo (fseek)
    struct arvore_bin* esq;
    struct arvore_bin* dir;
};

typedef struct arvore_bin Arvore_bin;

Arvore_bin* criar_arv_bin_pesquisa();

void destruir_arv_bin_pesquisa(Arvore_bin* a);

// Atualizado para receber a chave e o rrn
Arvore_bin* inserir_arv_bin_pesquisa(Arvore_bin* a, int chave, int rrn);

// Atualizado para retornar o RRN (-1 se não encontrar)
int buscar_arv_bin_pesquisa(Arvore_bin* a, int chave);

void buscar_maior_que(Arvore_bin* a, int valor_minimo, int* vetor_resultados, int* qtd_encontrados);

int obter_altura_bin(Arvore_bin* no);

void imprimir_pre_ordem(Arvore_bin* a);

void imprimir_em_ordem(Arvore_bin* a);

void imprimir_pos_ordem(Arvore_bin* a);

#endif