#include "../Arquivos-h/inicializadores.h"


Memorias *inicializaMem(){
    Memorias *memoria = malloc(256 * sizeof(Memorias));
    for(int i = 0; i < 256; i++){
        memoria[i].mem[0] = '\0';
        memoria[i].uso = -1;
    }
    return memoria;
}

RegistradoresAux *inicializaRegsAux(RegistradoresAux *RegsAux){
    RegsAux->registradorA = 0;
    RegsAux->registradorB = 0;
    RegsAux->registradorDados[0] = '\0';
    RegsAux->registradorULA = 0;
    RegsAux->PC = 0;
    return RegsAux;
}