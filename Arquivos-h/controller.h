#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "bibliotecas.h"
#include "memoria_instruc.h"
#include "registradores.h"
#include "decode.h"
#include "PC.h"
#include "ULA.h"
#include "imprimeSimulador.h"
#include "menu.h"

int controller(int op, int *state, instrucao **memoriaInst, int tamLinhas, int **regs, MemoriaDados **md, int *program_counter, type_instruc **instrucoesDecodificadas);
void backstep(int *state, instrucao **memoriaInst, int tamLinhas, int **regs, MemoriaDados **md, int *program_counter, type_instruc **instrucoesDecodificadas);

#endif
