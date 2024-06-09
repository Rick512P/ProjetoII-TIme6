#include "../Arquivos-h/imprimeRegsAux.h"


void imprimeRegsAux(RegistradoresAux *aux){
    printf("\nRegistrador de Instrucao: %s", aux->registradorInst);
    printf("\nRegistrador de Dados da Memoria: %s", aux->registradorDados);
    printf("\nRegistrador A: %s", aux->registradorA);
    printf("\nRegistrador B: %s", aux->registradorB);
    printf("\nRegistrador ULA saida: %d", aux->registradorULA);
}