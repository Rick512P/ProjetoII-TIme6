#ifndef PC_H
#define PC_H

#include "bibliotecas.h"

extern int program_counter;

int increment_PC( int *program_counter, int op);
void increment_State(int *StateForBack, int op);

#endif
