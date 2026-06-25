#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "TAD_arquivo.h"

double tempo_agora(void) {
    return (double) clock() / CLOCKS_PER_SEC;
}

int ler_registro_disco(FILE *arq, long num_reg, Registro *reg) {
    if (fseek(arq, num_reg * (long)sizeof(Registro), SEEK_SET) != 0) { 
        return 0;
    }

    return fread(reg, sizeof(Registro), 1, arq) == 1;
}

long total_registros(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / (long)sizeof(Registro);
}

void gerar_arquivo(int n) {
    int *mats = malloc(n * sizeof(int));

    if (mats == NULL) {
        fprintf(stderr, "Erro de memoria.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        mats[i] = 10000 + i;
    }

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int tmp = mats[i];
        mats[i] = mats[j];
        mats[j] = tmp;
    }

    const char *cursos[] = {"CC", "EC", "SI", "ES", "MAT"};

    FILE *arq = fopen(ARQUIVO, "wb");
    
    if (arq == NULL) {
        fprintf(stderr, "Erro ao criar arquivo.\n");
        free(mats);
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        Registro r;

        memset(&r, 0, sizeof(r));

        r.matricula = mats[i];
        r.cr = 2.0 + ((double) rand() / RAND_MAX) * 8.0;
        r.ano_ingresso = 1985 + rand() % 40;

        snprintf(r.nome, TAM_NOME, "Aluno_%05d", r.matricula);
        snprintf(r.curso, TAM_CURSO, "%s", cursos[rand() % 5]);

        fwrite(&r, sizeof(r), 1, arq);
    }

    fclose(arq);
    free(mats);

    printf("Arquivo gerado: %d registros, %zu bytes cada.\n", n, sizeof(Registro));
}

void sortear_chaves(FILE *arq, int *chaves, int qtd, long total) {
    long *idx = malloc(total * sizeof(long));

    if (idx == NULL) {
        fprintf(stderr, "Erro de memoria.\n");
        exit(1);
    }

    for (long i = 0; i < total; i++) {
        idx[i] = i;
    }

    for (int i = 0; i < qtd; i++) {
        long j = i + rand() % (total - i);

        long tmp = idx[i];
        idx[i] = idx[j];
        idx[j] = tmp;
    }

    Registro r;

    for (int i = 0; i < qtd; i++) {
        ler_registro_disco(arq, idx[i], &r);
        chaves[i] = r.matricula;
    }

    free(idx);
}

    long busca_sequencial_chave(FILE *arq, int chave_buscada, Registro *reg_encontrado){
    if (arq == NULL || reg_encontrado == NULL) {
        return -1;
    }

    rewind(arq);

    long rrn_atual = 0;

    while (fread(reg_encontrado, sizeof(Registro), 1, arq) == 1) {
        if (reg_encontrado->matricula == chave_buscada) {
            return rrn_atual;
        }
        rrn_atual++;
    }

    return -1;
}
