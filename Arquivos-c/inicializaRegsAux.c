#include "../Arquivos-h/inicializaRegsAux.h"

RegistradoresAux *inicializaRegsAux(){
    RegistradoresAux *RegsAux = malloc(sizeof(RegistradoresAux));
    RegsAux->registradorA = 0;
    RegsAux->registradorB = 0;
    *RegsAux->registradorDados = 0;
    *RegsAux->registradorInst = 0;
    RegsAux->registradorULA = 0;
    return RegsAux;
}

void imprimeRegsAux(RegistradoresAux *aux){
    printf("\nRegistrador de Instrucao: %s", aux->registradorInst);
    printf("\nRegistrador de Dados da Memoria: %s", aux->registradorDados);
    printf("\nRegistrador A: %d", aux->registradorA);
    printf("\nRegistrador B: %d", aux->registradorB);
    printf("\nRegistrador ULA saida: %d", aux->registradorULA);
}


