#ifndef ULA_H
#define ULA_H
#define BITS 17

#include "decode.h"
#include "memoria_instruc.h"
#include "parser.h"
#include "Memoria_Dados.h"
#include "bibliotecas.h"
#include "bin_to_decimal.h"
#include "decimal_to_bin.h"


int ULA(type_instruc **instrucoesDecodificadas, int *contador, MemoriaDados **md, int **regs);
void AND(char Source[], char Target[], char *Dest);
void OR(char Source[], char Target[], char *Dest);

#endif

