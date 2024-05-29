#ifndef __REGISTRADORES_H__
#define __REGISTRADORES_H__

#include "memoria_instruc.h"
#include "decode.h"
#include "ULA.h"
#include "bibliotecas.h"
#include "bin_to_decimal.h"


int escritaRegistradores(int **regs, int valor, char posicao[]);
int retornoRegs(int **regs, char posicao[]);
void imprimeRegistradores(int *regs);

#endif