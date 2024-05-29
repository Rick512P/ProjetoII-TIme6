#ifndef MEMORIA_DADOS_H
#define MEMORIA_DADOS_H


#include "bibliotecas.h"

void escreveDado(MemoriaDados **mem, int *contador, char *valor);
void imprimeDados(MemoriaDados *md, int tamLinhas);
void escreverArquivoMemoria(MemoriaDados *md);


#endif
