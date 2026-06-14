cmake_minimum_required(VERSION 4.2)
project(TP03_AED2final C)

set(CMAKE_C_STANDARD 11)

include_directories(src/main)

add_executable(TP03_AED2final
        src/main/main.c
        src/main/TAD_arquivo.c
        src/main/TAD_arquivo.h
        src/main/TAD_arv_bin_pesquisa.c
        src/main/TAD_arv_bin_pesquisa.h
        src/main/TAD_busca_sequencial.c
        src/main/TAD_busca_sequencial.h
        src/main/TAD_hash.c
        src/main/TAD_hash.h)
