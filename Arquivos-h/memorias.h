#ifndef __MEMORIA_INSTRUC_H__
#define __MEMORIA_INSTRUC_H__

#include "bibliotecas.h"
#include "decode.h"

#include "parser.h" //inclui arquivos de cabeçalho que estao no mesmo diretorio deste arquivo .c
#include "registradores.h"
#include "ULA.h"


type_instruc Memoria(RegistradoresAux *aux);
void imprimeMemoria(Memorias *memoria);
void escreveDado(Memorias *mem, int contador, char *valor);
void escreverArquivoMemoria(Memorias *memoria);
void remove_newline(char *line);



#endif