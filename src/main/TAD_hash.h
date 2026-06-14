#ifndef TAD_HASH_H
#define TAD_HASH_H

typedef struct HashNode {
    int chave;
    long numero_registro;
    struct HashNode* prox;
} HashNode;

typedef struct {
    HashNode** vetor;
    int tamanho;
    int colisoes_totais;
} HashTable;

HashTable* criar_tabela(int tamanho);
void liberar_tabela(HashTable* tabela);
int inserir_hash(HashTable* tabela, int chave, long numero_registro);
long buscar_hash(HashTable* tabela, int chave, int* colisoes_busca);

#endif