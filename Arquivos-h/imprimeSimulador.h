#ifndef __imprimeSimulador_h
#define __imprimeSimulador_h

#include "decode.h"
#include "memoria_instruc.h"
#include "registradores.h"
#include "bibliotecas.h"
#include "menu.h"


void imprimeSimulador(int tamLinhas, type_instruc **instrucoesDecodificadas, instrucao *memoriaInst);
void imprimeEstatisticas(instrucao *memoriaInst, int tamLinhas, type_instruc **instrucoesDecodificadas);

#endif