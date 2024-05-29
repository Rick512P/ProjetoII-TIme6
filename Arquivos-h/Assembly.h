#ifndef __ASSEMBLY_H__
#define __ASSEMBLY_H__

#include "bibliotecas.h"

void AsmCopy(type_instruc **instrucoesDecodificadas, Assembly **A, int tamLinhas);
char* decode_register(const char *reg);
void decode_function(const char *func, char *opcode);
int SaveASM(Assembly *A, int tamLinhas);
void imprimirASM(Assembly *A, int tamLinhas);
    
#endif
