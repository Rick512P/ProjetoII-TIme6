#ifndef __imprimeSimulador_h
#define __imprimeSimulador_h

#include "decode.h"
#include "memorias.h"
#include "registradores.h"
#include "bibliotecas.h"
#include "menu.h"


void imprimeSimulador(int tamLinhas, type_instruc **instrucoesDecodificadas, Memorias *memoria);
void imprimeEstatisticas(Memorias *memoria, int tamLinhas, type_instruc **instrucoesDecodificadas, int pc);
void imprimeRegsAux(RegistradoresAux *aux);

#endif