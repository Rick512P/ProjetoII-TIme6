#ifndef carrega_md_h
#define carrega_md_h

#include "bibliotecas.h"
#include "menu.h"

void remove_newline(char *line);
char* carregaDados(Memorias **memoria);
void escreverArquivoMemoria(Memorias *memoria);

#endif
