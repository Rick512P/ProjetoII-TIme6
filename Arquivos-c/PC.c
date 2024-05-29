#include "../Arquivos-h/PC.h"

// Função para incrementar o contador de programa
void increment_PC(int *program_counter, int op) {//PROGRAM_COUNTER COMO PONTEIRO POIS ELE RECEBERÁ O ENDEREÇO DE PROGRAM_COUNTER
    switch (op)
    {
     case 1:
        (*program_counter)++;
        break;
    case 2:
        (*program_counter)--;
        break;
    default:
        break;
    }
}