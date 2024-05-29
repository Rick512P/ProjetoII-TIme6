#ifndef __MEMORIA_INSTRUC_H__
#define __MEMORIA_INSTRUC_H__

#include "bibliotecas.h"
#include "../Arquivos-h/decode.h"

#include "../Arquivos-h/parser.h" //inclui arquivos de cabeçalho que estao no mesmo diretorio deste arquivo .c
#include "../Arquivos-h/registradores.h"
#include "../Arquivos-h/ULA.h"


type_instruc memInstruc(int contador, instrucao **memoriaInst, int *tamLinhas);
void imprimeMemInstruc(instrucao *memoriaInst, int tamLinhas);



#endif