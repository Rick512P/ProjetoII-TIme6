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


