#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "TAD_arv_bin_pesquisa.h"
#include "TAD_arquivo.h"
#include "TAD_hash.h"

// RRN = Relative Record Number
// RRN = numero relativo do registro no arquivo binario

int main() {
    srand((unsigned) time(NULL));

    int N = 10000;
    gerar_arquivo(N);

    FILE *arq = fopen(ARQUIVO, "rb");

    if (arq == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo.\n");
        return 1;
    }

    long total = total_registros(arq);

    clock_t inicio, fim;
    Registro reg_temp;
    long rrn_atual = 0;

    /* ============================================================
       QUESTAO 1: BUSCA USANDO ARVORE BINARIA PELO ATRIBUTO CHAVE
       ============================================================ */

    printf("\nConstruindo Indice da Arvore Binaria (Estrategia 1)...\n");
    inicio = clock();

    Arvore_bin *raiz = criar_arv_bin_pesquisa();

    for (long i = 0; i < total; i++) {
        if (ler_registro_disco(arq, i, &reg_temp)) {
            raiz = inserir_arv_bin_pesquisa(raiz, reg_temp.matricula, (int) i);
        }
    }

    fim = clock();

    if (total <= 20) {
        printf("\n--- DICA 2: PARES <CHAVE, RRN> NA ARVORE (Em Ordem) ---\n");
        imprimir_em_ordem(raiz);
        printf("\n-------------------------------------------------------\n\n");
    }

    printf("Tempo de construcao do indice da Arvore: %.6f s\n\n",
           (double)(fim - inicio) / CLOCKS_PER_SEC);

    /* ============================================================
       QUESTAO 2: BUSCA USANDO HASH PELO ATRIBUTO CHAVE
       ============================================================ */

    printf("\nConstruindo Indice Hash (Estrategia 2)...\n");
    inicio = clock();

    Hash tabela_hash;
    hash_inicializar(&tabela_hash);

    for (long i = 0; i < total; i++) {
        if (ler_registro_disco(arq, i, &reg_temp)) {
            hash_inserir(&tabela_hash, reg_temp.matricula, (int) i);
        }
    }

    fim = clock();

    if (total <= 20) {
        printf("\n--- DICA 2: PARES <CHAVE, RRN> NA HASH ---\n");
        hash_mostrar(&tabela_hash);
        printf("\n------------------------------------------\n\n");
    }

    printf("Tempo de construcao do indice Hash: %.6f s\n",
           (double)(fim - inicio) / CLOCKS_PER_SEC);
    printf("Quantidade de colisoes na Hash: %d\n\n", tabela_hash.colisoes);

    /* Sorteia 30 chaves iguais para as estrategias 1, 2 e 3 */
    int chaves_teste[NUM_BUSCAS];
    sortear_chaves(arq, chaves_teste, NUM_BUSCAS, total);

    /* ============================================================
       EXECUCAO DA ESTRATEGIA 1
       ============================================================ */

    printf("=== Estrategia 1: Busca via Arvore (chave = matricula) ===\n");
    printf("%-5s %-10s %-15s %s\n", "#", "Chave", "Tempo (s)", "Nome encontrado");
    printf("----------------------------------------------------\n");

    double soma_tempo_q1 = 0.0;

    for (int i = 0; i < NUM_BUSCAS; i++) {
        clock_t ini_busca = clock();

        int num_reg = buscar_arv_bin_pesquisa(raiz, chaves_teste[i]);
        int achou = (num_reg >= 0) && ler_registro_disco(arq, num_reg, &reg_temp);

        clock_t fim_busca = clock();

        double dt = (double)(fim_busca - ini_busca) / CLOCKS_PER_SEC;
        soma_tempo_q1 += dt;

        if (achou) {
            printf("[%2d]  %-10d  %.9f  %s\n",
                   i + 1, chaves_teste[i], dt, reg_temp.nome);
        } else {
            printf("[%2d]  %-10d  %.9f  NAO ENCONTRADO\n",
                   i + 1, chaves_teste[i], dt);
        }
    }

    double media_q1 = soma_tempo_q1 / NUM_BUSCAS;

    printf("----------------------------------------------------\n");
    printf("Tempo medio da Estrategia 1 (%d buscas): %.9f s\n\n",
           NUM_BUSCAS, media_q1);

    /* ============================================================
       EXECUCAO DA ESTRATEGIA 2
       ============================================================ */

    printf("=== Estrategia 2: Busca via Hash (chave = matricula) ===\n");
    printf("%-5s %-10s %-15s %s\n", "#", "Chave", "Tempo (s)", "Nome encontrado");
    printf("----------------------------------------------------\n");

    double soma_tempo_q2 = 0.0;

    for (int i = 0; i < NUM_BUSCAS; i++) {
        clock_t ini_busca = clock();

        int num_reg = hash_buscar(&tabela_hash, chaves_teste[i]);
        int achou = (num_reg >= 0) && ler_registro_disco(arq, num_reg, &reg_temp);

        clock_t fim_busca = clock();

        double dt = (double)(fim_busca - ini_busca) / CLOCKS_PER_SEC;
        soma_tempo_q2 += dt;

        if (achou) {
            printf("[%2d]  %-10d  %.9f  %s\n",
                   i + 1, chaves_teste[i], dt, reg_temp.nome);
        } else {
            printf("[%2d]  %-10d  %.9f  NAO ENCONTRADO\n",
                   i + 1, chaves_teste[i], dt);
        }
    }

    double media_q2 = soma_tempo_q2 / NUM_BUSCAS;

    printf("----------------------------------------------------\n");
    printf("Tempo medio da Estrategia 2 (%d buscas): %.9f s\n",
           NUM_BUSCAS, media_q2);
    printf("Total de colisoes na Hash: %d\n\n", tabela_hash.colisoes);

    /* ============================================================
       QUESTAO 3: BUSCA SEQUENCIAL NO ARQUIVO PELO ATRIBUTO CHAVE
       ============================================================ */

    printf("=== Estrategia 3: Busca Sequencial no Arquivo (chave = matricula) ===\n");
    printf("%-5s %-10s %-15s %s\n", "#", "Chave", "Tempo (s)", "Nome encontrado");
    printf("----------------------------------------------------\n");

    double soma_tempo_q3 = 0.0;

    for (int i = 0; i < NUM_BUSCAS; i++) {
        clock_t ini_busca = clock();

        int achou = 0;
        rewind(arq);

        while (fread(&reg_temp, sizeof(Registro), 1, arq) == 1) {
            if (reg_temp.matricula == chaves_teste[i]) {
                achou = 1;
                break;
            }
        }

        clock_t fim_busca = clock();

        double dt = (double)(fim_busca - ini_busca) / CLOCKS_PER_SEC;
        soma_tempo_q3 += dt;

        if (achou) {
            printf("[%2d]  %-10d  %.9f  %s\n",
                   i + 1, chaves_teste[i], dt, reg_temp.nome);
        } else {
            printf("[%2d]  %-10d  %.9f  NAO ENCONTRADO\n",
                   i + 1, chaves_teste[i], dt);
        }
    }

    double media_q3 = soma_tempo_q3 / NUM_BUSCAS;

    printf("----------------------------------------------------\n");
    printf("Tempo medio da Estrategia 3 (%d buscas): %.9f s\n\n",
           NUM_BUSCAS, media_q3);

    /* ============================================================
       QUESTAO 4: BUSCA POR FAIXA USANDO ARVORE
       atributo nao-chave: ano_ingresso
       ============================================================ */

    printf("\n--- MONTANDO INDICE DA ESTRATEGIA 4 ---\n");

    Arvore_bin *raiz_ano = criar_arv_bin_pesquisa();

    rewind(arq);
    rrn_atual = 0;

    while (fread(&reg_temp, sizeof(Registro), 1, arq) == 1) {
        raiz_ano = inserir_arv_bin_pesquisa(raiz_ano, reg_temp.ano_ingresso, (int) rrn_atual);
        rrn_atual++;
    }

    int anos_teste[NUM_BUSCAS];

    for (int i = 0; i < NUM_BUSCAS; i++) {
        anos_teste[i] = 1990 + i;
    }

    printf("=== Estrategia 4: Busca via Arvore (Ano > X) ===\n");

    double soma_tempo_q4 = 0.0;

    for (int i = 0; i < NUM_BUSCAS; i++) {
        int ano_minimo = anos_teste[i];
        int *resultados_rrn = malloc(total * sizeof(int));
        int qtd_encontrados = 0;

        if (resultados_rrn == NULL) {
            printf("Erro de memoria.\n");
            exit(1);
        }

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
                   i + 1, ano_minimo, qtd_encontrados, dt_q4);
        }

        if (i == 5) {
            printf("... executando as demais buscas ...\n");
        }

        free(resultados_rrn);
    }

    double media_q4 = soma_tempo_q4 / NUM_BUSCAS;

    printf("Tempo MEDIO da Estrategia 4 (%d buscas): %.9f s\n",
           NUM_BUSCAS, media_q4);

    /* ============================================================
       QUESTAO 5: BUSCA POR FAIXA COM BUSCA SEQUENCIAL
       ============================================================ */

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
                   i + 1, ano_minimo, qtd_encontrados_seq, dt_q5);
        }
    }

    double media_q5 = soma_tempo_q5 / NUM_BUSCAS;

    printf("Tempo MEDIO da Estrategia 5 (%d buscas): %.9f s\n",
           NUM_BUSCAS, media_q5);

    /* ============================================================
       COMPARATIVO FINAL
       ============================================================ */

    printf("\n====================================================\n");
    printf("        COMPARATIVO FINAL DE DESEMPENHO (MEDIAS)    \n");
    printf("====================================================\n");
    printf("Estrategia 1 (Arvore por matricula) : %.9f seg\n", media_q1);
    printf("Estrategia 2 (Hash por matricula)   : %.9f seg\n", media_q2);
    printf("Estrategia 3 (Sequencial matricula) : %.9f seg\n", media_q3);
    printf("Estrategia 4 (Arvore por ano > X)   : %.9f seg\n", media_q4);
    printf("Estrategia 5 (Sequencial ano > X)   : %.9f seg\n", media_q5);
    printf("Colisoes na Hash                    : %d\n", tabela_hash.colisoes);
    printf("====================================================\n");

    destruir_arv_bin_pesquisa(raiz);
    destruir_arv_bin_pesquisa(raiz_ano);
    hash_destruir(&tabela_hash);
    fclose(arq);

    return 0;
}
