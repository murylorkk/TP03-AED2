#ifndef TAD_BUSCA_SEQUENCIAL_H
#define TAD_BUSCA_SEQUENCIAL_H

#include <stdio.h>
#include "TAD_arquivo.h"

long busca_sequencial_chave(FILE *arq, int chave_buscada, Registro *reg_encontrado);

#endif