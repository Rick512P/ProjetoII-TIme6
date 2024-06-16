#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "bibliotecas.h"
#include "memorias.h"
#include "registradores.h"
#include "decode.h"
#include "PC.h"
#include "ULA.h"
#include "imprimeSimulador.h"
#include "menu.h"
#include "inicializadores.h"

int controller(int op, int *StateForBack, int NumeroLinhas, int *regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal, int ProxEtapa);
void backstep(int *StateForBack, int tamLinhas, int *regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal);
#endif
