#include "../Arquivos-h/inicializadores.h"


Memorias *inicializaMem(){
    Memorias *md = malloc(256 * sizeof(Memorias));
    for(int i = 0; i < 256; i++){
        md[i].mem[0] = '\0';
        md[i].uso = '\0';
    }
    return md;
}

RegistradoresAux *inicializaRegsAux(){
    RegistradoresAux *RegsAux = malloc(sizeof(RegistradoresAux));
    RegsAux->registradorA = 0;
    RegsAux->registradorB = 0;
    RegsAux->registradorDados[0] = '\0';
    RegsAux->registradorInst[0] = '\0';
    RegsAux->registradorULA = 0;
    return RegsAux;
}