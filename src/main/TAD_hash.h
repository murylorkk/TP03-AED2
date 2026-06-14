#ifndef TAD_HASH_H
#define TAD_HASH_H

#define TAM_HASH 20011

typedef struct no_hash {
    int chave;          // matricula
    int rrn;            // posicao do registro no arquivo
    struct no_hash *prox;
} NoHash;

typedef struct {
    NoHash *tabela[TAM_HASH];
    int colisoes;
} Hash;

/* Inicializacao */
void hash_inicializar(Hash *h);

/* Insercao */
void hash_inserir(Hash *h, int chave, int rrn);

/* Busca */
int hash_buscar(Hash *h, int chave);

/* Impressao */
void hash_mostrar(Hash *h);

/* Liberacao de memoria */
void hash_destruir(Hash *h);

#endif
