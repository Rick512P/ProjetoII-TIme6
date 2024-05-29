#include "../Arquivos-h/imprimeSimulador.h"


void imprimeSimulador(int tamLinhas, type_instruc **instrucoesDecodificadas, instrucao *memoriaInst){
    for(int i=0;i<tamLinhas;i++){
            printf("\n\nInstrucao %d de %d: [%s]", i+1, tamLinhas, memoriaInst[i].instruc);
            if (strcmp((*instrucoesDecodificadas)[i].opcode, "0000") == 0){
                printf("\n======TIPO R=========\n");
                printf("opcode[%s]\t", (*instrucoesDecodificadas)[i].opcode);
                printf("rs[%s]\t\t", (*instrucoesDecodificadas)[i].rs);
                printf("rt[%s]\t\t", (*instrucoesDecodificadas)[i].rt);
                printf("rd[%s]\t\t", (*instrucoesDecodificadas)[i].rd);
                printf("funct[%s]\t\t", (*instrucoesDecodificadas)[i].funct);
                printf("\n======================\n");
            }
            else if (strcmp((*instrucoesDecodificadas)[i].opcode, "0010") == 0){
                printf("\n========TIPO J========\n");
                printf("opcode[%s]\t", (*instrucoesDecodificadas)[i].opcode);
                printf("addr[%s]\t\t", (*instrucoesDecodificadas)[i].addr);
                printf("\n======================\n");
            } 

            else{
                printf("\n\n=======TIPO I=========\n");
                printf("opcode[%s]\t", (*instrucoesDecodificadas)[i].opcode);
                printf("rs[%s]\t\t", (*instrucoesDecodificadas)[i].rs);
                printf("rt[%s]\t\t", (*instrucoesDecodificadas)[i].rt);
                printf("imm[%s]\t\t", (*instrucoesDecodificadas)[i].imm);
                
                printf("\n======================\n");
            }

               
    }
}


void imprimeEstatisticas(instrucao *memoriaInst, int tamLinhas, type_instruc **instrucoesDecodificadas){
    if (memoriaInst == NULL) {
                fprintf(stderr, "Falha ao obter instruções.\n");
            }
    int r=0, i=0, j=0, instAri=0, instLogic=0, instDesvio=0, instAcessoMem=0;
    for(int y=0;y<tamLinhas;y++){
        if (strncmp(memoriaInst[y].instruc, "0000", 4) == 0){ //compara os 4 primeiros numeros de memoriaInst com "0000"
            r++;
            if ((strcmp((*instrucoesDecodificadas)[y].funct, "000")) || (strcmp((*instrucoesDecodificadas)[y].funct, "010") == 0))
                instAri++;
            else
                instLogic++;
        }
        else if (strncmp(memoriaInst[y].instruc, "0100", 4) == 0 || strncmp(memoriaInst[y].instruc, "1011", 4) == 0 || strncmp(memoriaInst[y].instruc, "1111", 4) == 0 || strncmp(memoriaInst[y].instruc, "0110", 4) == 0 || strncmp(memoriaInst[y].instruc, "1000", 4) == 0){
            i++;
            if (strncmp(memoriaInst[y].instruc, "0100", 4) == 0)
                instAri++;
            else if ((strncmp(memoriaInst[y].instruc, "1011", 4) == 0) || strncmp(memoriaInst[y].instruc, "1111", 4) == 0)
                instAcessoMem++;
            else if (strncmp(memoriaInst[y].instruc, "1000", 4) == 0)
                instDesvio++;



        }  
        else if (strncmp(memoriaInst[y].instruc, "0010", 4) == 0){
            j++;
            instDesvio++;
        }
            
    }
    printf("O numero de instrucoes e de %d\n", tamLinhas);
    printf("Observa-se: \n%d instrucoes do tipo R\n%d instrucoes do tipo I\n%d instrucoes do tipo J\n", r,i,j);
    printf("Onde %d instrucoes sao da classe Logica, %d Aritmetica, %d Desvio e %d Acesso a Memoria de Dados\n", instLogic, instAri, instDesvio, instAcessoMem);
}