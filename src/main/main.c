#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "TAD_arv_bin_pesquisa.h"
#include "TAD_arquivo.h"
#include "TAD_hash.h"

int main(){
    srand((unsigned)time(NULL));

    int N = 100;
    gerar_arquivo(N);

    FILE *arq = fopen(ARQUIVO, "rb");
    if (arq == NULL){
        fprintf(stderr, "Erro ao abrir arquivo.\n");
        return 1;
    }
    long total = total_registros(arq);

    clock_t inicio, fim;
    Registro reg_temp;
    long rrn_atual = 0;

    printf("\nConstruindo Indice da Arvore Binaria (Estrategia 1)...\n");
    inicio = clock();

    Arvore_bin *raiz = criar_arv_bin_pesquisa();

    for (long i = 0; i < total; i++) {
        if (ler_registro_disco(arq, i, &reg_temp)) {
            raiz = inserir_arv_bin_pesquisa(raiz, reg_temp.matricula, i);
        }
    }

    if (total <= 20) {
        printf("\n--- DICA 2: PARES <CHAVE, RRN> NA ARVORE (Em Ordem) ---\n");
        imprimir_em_ordem(raiz);
        printf("\n-------------------------------------------------------\n\n");
    }

    fim = clock();
    printf("Tempo de construcao do indice: %.6f s\n\n", (double)(fim - inicio) / CLOCKS_PER_SEC);

    int chaves_teste[NUM_BUSCAS];
    sortear_chaves(arq, chaves_teste, NUM_BUSCAS, total);

    printf("=== Estrategia 1: Busca via Arvore (chave = matricula) ===\n");
    printf("%-5s %-10s %-15s %s\n", "#", "Chave", "Tempo (s)", "Nome encontrado");
    printf("----------------------------------------------------\n");

    double soma_tempo = 0.0;

    for (int i = 0; i < NUM_BUSCAS; i++) {
        clock_t ini_busca = clock();

        long num_reg = buscar_arv_bin_pesquisa(raiz, chaves_teste[i]);

        int achou = (num_reg >= 0) && ler_registro_disco(arq, num_reg, &reg_temp);

        clock_t fim_busca = clock();
        double dt = (double)(fim_busca - ini_busca) / CLOCKS_PER_SEC;
        soma_tempo += dt;

        if (achou)
            printf("[%2d]  %-10d  %.9f  %s\n", i+1, chaves_teste[i], dt, reg_temp.nome);
        else
            printf("[%2d]  %-10d  %.9f  NAO ENCONTRADO\n", i+1, chaves_teste[i], dt);
    }

    printf("----------------------------------------------------\n");
    double media_q1 = soma_tempo / NUM_BUSCAS;
    printf("Tempo medio da Estrategia 1 (%d buscas): %.9f s\n", NUM_BUSCAS, media_q1);

    printf("\nConstruindo Indice da Tabela Hash (Estrategia 2)...\n");
    inicio = clock();

    HashTable *tabela = criar_tabela((total * 2) + 7);

    rewind(arq);
    rrn_atual = 0;
    while (fread(&reg_temp, sizeof(Registro), 1, arq) == 1) {
        inserir_hash(tabela, reg_temp.matricula, rrn_atual);
        rrn_atual++;
    }

    fim = clock();
    printf("Tempo de construcao do indice Hash: %.6f s\n\n", (double)(fim - inicio) / CLOCKS_PER_SEC);

    printf("=== Estrategia 2: Busca via Hash (chave = matricula) ===\n");
    printf("%-5s %-10s %-15s %-10s %s\n", "#", "Chave", "Tempo (s)", "Colisoes", "Nome encontrado");
    printf("-----------------------------------------------------------------\n");

    double soma_tempo_q2 = 0.0;
    int colisoes_busca = 0;

    for (int i = 0; i < NUM_BUSCAS; i++) {
        clock_t ini_busca = clock();

        long num_reg = buscar_hash(tabela, chaves_teste[i], &colisoes_busca);
        int achou = (num_reg >= 0) && ler_registro_disco(arq, num_reg, &reg_temp);

        clock_t fim_busca = clock();
        double dt = (double)(fim_busca - ini_busca) / CLOCKS_PER_SEC;
        soma_tempo_q2 += dt;

        if (achou)
            printf("[%2d]  %-10d  %.9f  %-10d %s\n", i+1, chaves_teste[i], dt, colisoes_busca, reg_temp.nome);
        else
            printf("[%2d]  %-10d  %.9f  %-10d NAO ENCONTRADO\n", i+1, chaves_teste[i], dt, colisoes_busca);
    }

    printf("-----------------------------------------------------------------\n");
    double media_q2 = soma_tempo_q2 / NUM_BUSCAS;
    printf("Tempo medio da Estrategia 2 (%d buscas): %.9f s\n", NUM_BUSCAS, media_q2);

    printf("\n=== Estrategia 3: Busca Sequencial (chave = matricula) ===\n");
    printf("%-5s %-10s %-15s %s\n", "#", "Chave", "Tempo (s)", "Nome encontrado");
    printf("----------------------------------------------------\n");

    double soma_tempo_q3 = 0.0;

    for (int i = 0; i < NUM_BUSCAS; i++) {
        clock_t ini_busca = clock();

        rewind(arq);
        int achou = 0;
        while (fread(&reg_temp, sizeof(Registro), 1, arq) == 1) {
            if (reg_temp.matricula == chaves_teste[i]) {
                achou = 1;
                break;
            }
        }

        clock_t fim_busca = clock();
        double dt = (double)(fim_busca - ini_busca) / CLOCKS_PER_SEC;
        soma_tempo_q3 += dt;

        if (achou)
            printf("[%2d]  %-10d  %.9f  %s\n", i+1, chaves_teste[i], dt, reg_temp.nome);
        else
            printf("[%2d]  %-10d  %.9f  NAO ENCONTRADO\n", i+1, chaves_teste[i], dt);
    }

    printf("----------------------------------------------------\n");
    double media_q3 = soma_tempo_q3 / NUM_BUSCAS;
    printf("Tempo medio da Estrategia 3 (%d buscas): %.9f s\n", NUM_BUSCAS, media_q3);

    printf("\n--- MONTANDO INDICE DA ESTRATEGIA 4 ---\n");
    Arvore_bin* raiz_ano = criar_arv_bin_pesquisa();

    rewind(arq);
    rrn_atual = 0;

    while (fread(&reg_temp, sizeof(Registro), 1, arq) == 1) {
        raiz_ano = inserir_arv_bin_pesquisa(raiz_ano, reg_temp.ano_ingresso, rrn_atual);
        rrn_atual++;
    }

    int anos_teste[NUM_BUSCAS];
    for(int i = 0; i < NUM_BUSCAS; i++) {
        anos_teste[i] = 1990 + i;
    }

    printf("=== Estrategia 4: Busca via Arvore (Ano > X) ===\n");
    double soma_tempo_q4 = 0.0;

    for (int i = 0; i < NUM_BUSCAS; i++) {
        int ano_minimo = anos_teste[i];
        int resultados_rrn[10000];
        int qtd_encontrados = 0;

        clock_t inicio_q4 = clock();

        buscar_maior_que(raiz_ano, ano_minimo, resultados_rrn, &qtd_encontrados);

        for (int j = 0; j < qtd_encontrados; j++) {
            ler_registro_disco(arq, resultados_rrn[j], &reg_temp);
        }

        clock_t fim_q4 = clock();
        double dt_q4 = (double)(fim_q4 - inicio_q4) / CLOCKS_PER_SEC;
        soma_tempo_q4 += dt_q4;

        if (i < 5 || i == NUM_BUSCAS - 1) {
            printf("[%2d] Buscando Ano > %d | Encontrados: %-4d | Tempo: %.9f s\n",
                   i+1, ano_minimo, qtd_encontrados, dt_q4);
        }
        if (i == 5) printf("... executando as demais buscas ...\n");
    }

    double media_q4 = soma_tempo_q4 / NUM_BUSCAS;
    printf("Tempo MEDIO da Estrategia 4 (%d buscas): %.9f s\n", NUM_BUSCAS, media_q4);

    printf("\n=== Estrategia 5: Busca Sequencial (Ano > X) ===\n");
    double soma_tempo_q5 = 0.0;

    for (int i = 0; i < NUM_BUSCAS; i++) {
        int ano_minimo = anos_teste[i];
        int qtd_encontrados_seq = 0;

        clock_t inicio_q5 = clock();

        rewind(arq);

        while (fread(&reg_temp, sizeof(Registro), 1, arq) == 1) {
            if (reg_temp.ano_ingresso > ano_minimo) {
                qtd_encontrados_seq++;
            }
        }

        clock_t fim_q5 = clock();
        double dt_q5 = (double)(fim_q5 - inicio_q5) / CLOCKS_PER_SEC;
        soma_tempo_q5 += dt_q5;

        if (i < 5 || i == NUM_BUSCAS - 1) {
            printf("[%2d] Buscando Ano > %d | Encontrados: %-4d | Tempo: %.9f s\n",
                   i+1, ano_minimo, qtd_encontrados_seq, dt_q5);
        }
    }

    double media_q5 = soma_tempo_q5 / NUM_BUSCAS;
    printf("Tempo MEDIO da Estrategia 5 (%d buscas): %.9f s\n", NUM_BUSCAS, media_q5);

    printf("\n====================================================\n");
    printf("        COMPARATIVO FINAL DE DESEMPENHO (MEDIAS)      \n");
    printf("====================================================\n");
    printf("Estrategia 1 (Arvore Chave)     : %.9f seg\n", media_q1);
    printf("Estrategia 2 (Hash Chave)       : %.9f seg\n", media_q2);
    printf("Estrategia 3 (Sequencial Chave) : %.9f seg\n", media_q3);
    printf("----------------------------------------------------\n");
    printf("Estrategia 4 (Arvore de Indice) : %.9f seg\n", media_q4);
    printf("Estrategia 5 (Varredura no HD)  : %.9f seg\n", media_q5);
    printf("====================================================\n");

    destruir_arv_bin_pesquisa(raiz);
    destruir_arv_bin_pesquisa(raiz_ano);
    liberar_tabela(tabela);
    fclose(arq);

    return 0;
}