#include "TAD_busca_sequencial.h"

long busca_sequencial_chave(FILE *arq, int chave_buscada, Registro *reg_encontrado) {
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