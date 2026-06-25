#ifndef TAD_ARQUIVO_H
#define TAD_ARQUIVO_H

#include <stdio.h>

#define TAM_NOME   60
#define TAM_CURSO  30
#define ARQUIVO    "alunos.dat"
#define NUM_BUSCAS 30

typedef struct {
    int    matricula; // atributo chave
    char   nome[TAM_NOME];
    char   curso[TAM_CURSO];
    double cr;
    int    ano_ingresso;
} Registro;

int ler_registro_disco(FILE *arq, long num_reg, Registro *reg);

long total_registros(FILE *arq);

void gerar_arquivo(int n);

void sortear_chaves(FILE *arq, int *chaves, int qtd, long total);

double tempo_agora(void);

long busca_sequencial_chave(FILE *arq, int chave_buscada, Registro *reg_encontrado);

#endif
