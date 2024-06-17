#include "../Arquivos-h/registradores.h"



int retornoRegs(int *regs, char posicao[]){
    int indice;
        indice = bin_to_decimal(posicao);
        if(indice >= 8 || indice<0){
            fprintf(stderr, "ERRO. Nenhum Registrador encontrado.");
            return -1;
        }
        return regs[indice];
}

int escritaRegistradores(int *regs, int valor, char posicao[]){
       int indice;
        indice = bin_to_decimal(posicao);
        if(indice >= 8){
            fprintf(stderr, "ERRO. Nenhum Registrador encontrado.");
            return -1;
        }
            
        regs[indice] = valor;

            
}

void imprimeRegistradores(int *regs){
    printf("\n");
    printf("Registrador $%d: %d\t", 0, regs[0]);
    printf("Registrador $%d: %d\t", 1, regs[1]);
    printf("Registrador $%d: %d\t", 2, regs[2]);
    printf("Registrador $%d: %d\n", 3, regs[3]);
    printf("Registrador $%d: %d\t", 4, regs[4]);
    printf("Registrador $%d: %d\t", 5, regs[5]);
    printf("Registrador $%d: %d\t", 6, regs[6]);
    printf("Registrador $%d: %d\n", 7, regs[7]);

    printf("\n");
}



