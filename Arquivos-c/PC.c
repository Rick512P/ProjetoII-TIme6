#include "../Arquivos-h/PC.h"

// Função para incrementar o contador de programa
void increment_PC(int *program_counter, int op) {//PROGRAM_COUNTER COMO PONTEIRO POIS ELE RECEBERÁ O ENDEREÇO DE PROGRAM_COUNTER
    switch (op)
    {
     case 1:
        (*program_counter)++;
        break;
    case 2:
        (*program_counter) = 0;
        break;
    default:
        break;
    }
}

// Função para incrementar o estado para backStep
void increment_State(int *StateForBack, int op){
    switch (op)
    {
     case 1:
        (*StateForBack)++;
        break;
    case 2:
        (*StateForBack) = -1;
        break;
    default:
        break;
    }
}