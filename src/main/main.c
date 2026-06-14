#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "TAD_arv_bin_pesquisa.h"
#include "TAD_arquivo.h"

//RRN = Relative Record Number (representa o "indice" em que um dado foi salvo dentro do arquivo binário)
//RRN = Número Relativo do Registro

int main(){
    srand((unsigned)time(NULL));

    int N = 15;
    gerar_arquivo(N);

    FILE *arq = fopen(ARQUIVO, "rb");
    if (arq == NULL){ 
        fprintf(stderr, "Erro ao abrir arquivo.\n"); return 1;
    }
    long total = total_registros(arq);

    clock_t inicio, fim;
    Registro reg_temp;
    long rrn_atual = 0;

    // questão 1: busca usando a árvore binária de pesquisa para o atributo chave;
    printf("\nConstruindo Indice da Arvore Binaria (Estrategia 1)...\n");
    inicio = clock();

    Arvore_bin *raiz = criar_arv_bin_pesquisa();

    for (long i = 0; i < total; i++) {
        if (ler_registro_disco(arq, i, &reg_temp)) {
            raiz = inserir_arv_bin_pesquisa(raiz, reg_temp.matricula, i);
        }
    }

    // =========================================================================
    // DICA 2: IMPRESSÃO DOS PARES PARA ARQUIVOS PEQUENOS
    // =========================================================================
    if (total <= 20) {
        printf("\n--- DICA 2: PARES <CHAVE, RRN> NA ARVORE (Em Ordem) ---\n");
        imprimir_em_ordem(raiz); 
        printf("\n-------------------------------------------------------\n\n");
    }

    fim = clock();
    printf("Tempo de construcao do indice: %.6f s\n\n", (double)(fim - inicio) / CLOCKS_PER_SEC);

    int chaves_teste[NUM_BUSCAS]; // Sorteia 30 chaves para fazer os testes
    sortear_chaves(arq, chaves_teste, NUM_BUSCAS, total);

    printf("=== Estrategia 1: Busca via Arvore (chave = matricula) ===\n");
    printf("%-5s %-10s %-15s %s\n", "#", "Chave", "Tempo (s)", "Nome encontrado");
    printf("----------------------------------------------------\n");

    double soma_tempo = 0.0;
    
    for (int i = 0; i < NUM_BUSCAS; i++) {
        clock_t ini_busca = clock();

        long num_reg = buscar_arv_bin_pesquisa(raiz, chaves_teste[i]);
        
        // Vai no disco ler os dados do aluno usando ponteiro
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
    printf("Tempo medio da Estrategia 1 (%d buscas): %.9f s\n", NUM_BUSCAS, soma_tempo / NUM_BUSCAS);


   // =========================================================================
    // ESTRATÉGIA 4: BUSCA POR FAIXA (ANO DE INGRESSO > X) USANDO ÁRVORE BINÁRIA
    // =========================================================================
    printf("\n--- MONTANDO INDICE DA ESTRATEGIA 4 ---\n");
    Arvore_bin* raiz_ano = criar_arv_bin_pesquisa();
    
    rewind(arq); 
    rrn_atual = 0;

    // Lendo para montar a árvore secundária
    while (fread(&reg_temp, sizeof(Registro), 1, arq) == 1) {
        raiz_ano = inserir_arv_bin_pesquisa(raiz_ano, reg_temp.ano_ingresso, rrn_atual);
        rrn_atual++;
    }

    // Criando 30 valores DISTINTOS para testar (ex: de 1990 a 2019)
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
        
        // 1. A árvore devolve o vetor preenchido com os RRNs
        buscar_maior_que(raiz_ano, ano_minimo, resultados_rrn, &qtd_encontrados);
        
        // 2. Leitura no disco dos resultados (sem printf para não travar o terminal)
        for (int j = 0; j < qtd_encontrados; j++) {
            ler_registro_disco(arq, resultados_rrn[j], &reg_temp);
        }

        clock_t fim_q4 = clock();
        double dt_q4 = (double)(fim_q4 - inicio_q4) / CLOCKS_PER_SEC;
        soma_tempo_q4 += dt_q4;

        // Imprime apenas algumas execuções para o professor ver que está rodando
        if (i < 5 || i == NUM_BUSCAS - 1) {
            printf("[%2d] Buscando Ano > %d | Encontrados: %-4d | Tempo: %.9f s\n", 
                   i+1, ano_minimo, qtd_encontrados, dt_q4);
        }
        if (i == 5) printf("... executando as demais buscas ...\n");
    }

    double media_q4 = soma_tempo_q4 / NUM_BUSCAS;
    printf("Tempo MEDIO da Estrategia 4 (%d buscas): %.9f s\n", NUM_BUSCAS, media_q4);

    // =========================================================================
    // ESTRATÉGIA 5: BUSCA POR FAIXA COM BUSCA SEQUENCIAL DIRETO NO DISCO
    // =========================================================================
    printf("\n=== Estrategia 5: Busca Sequencial (Ano > X) ===\n");
    double soma_tempo_q5 = 0.0;
    
    for (int i = 0; i < NUM_BUSCAS; i++) {
        int ano_minimo = anos_teste[i];
        int qtd_encontrados_seq = 0;
        
        clock_t inicio_q5 = clock();
        
        // OBRIGATÓRIO: Reposicionar o ponteiro do disco no início a cada nova busca!
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
    
    // --- RESULTADO FINAL EXIGIDO PELO PDF ---
    printf("\n====================================================\n");
    printf("        COMPARATIVO FINAL DE DESEMPENHO (MEDIAS)      \n");
    printf("====================================================\n");
    printf("Estrategia 4 (Arvore de Indice) : %.9f seg\n", media_q4);
    printf("Estrategia 5 (Varredura no HD)  : %.9f seg\n", media_q5);
    printf("====================================================\n");

    // =========================================================================
    // LIMPEZA DE MEMÓRIA E FECHAMENTO DO ARQUIVO
    // =========================================================================
    destruir_arv_bin_pesquisa(raiz);
    destruir_arv_bin_pesquisa(raiz_ano);
    fclose(arq);

    return 0; 
}
