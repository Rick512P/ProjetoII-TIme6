#include "menu.h"

//extende 6bits para 8bits
char *extensor(char *address){
    char bit;
    char sinalExtendido[9];

    bit = address[0]; //copia o bit mais significativo

    strcat(sinalExtendido, bit);
    strcat(sinalExtendido, bit);
    strncat(sinalExtendido, address, sizeof(sinalExtendido) - strlen(sinalExtendido) - 1);//espaço restante em sinalExtendido (incluindo espaço para o terminador nulo).
    return sinalExtendido;
}