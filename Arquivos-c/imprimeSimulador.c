#include "../Arquivos-h/imprimeSimulador.h"


void imprimeSimulador(int tamLinhas, type_instruc *instrucoesDecodificadas, Memorias *memoria){
    for(int i=0;i<tamLinhas;i++){
        if(memoria[i].uso == 0){ 
            printf("\n\nInstrucao %d de %d: [%s]", i+1, tamLinhas, memoria[i].mem);
            if (strcmp(instrucoesDecodificadas[i].opcode, "0000") == 0){
                printf("\n======TIPO R=========\n");
                printf("opcode[%s]\t", instrucoesDecodificadas[i].opcode);
                printf("rs[%s]\t\t", instrucoesDecodificadas[i].rs);
                printf("rt[%s]\t\t", instrucoesDecodificadas[i].rt);
                printf("rd[%s]\t\t", instrucoesDecodificadas[i].rd);
                printf("funct[%s]\t\t", instrucoesDecodificadas[i].funct);
                printf("\n======================\n");
            }
            else if (strcmp(instrucoesDecodificadas[i].opcode, "0010") == 0){
                printf("\n========TIPO J========\n");
                printf("opcode[%s]\t", instrucoesDecodificadas[i].opcode);
                printf("addr[%s]\t\t", instrucoesDecodificadas[i].addr);
                printf("\n======================\n");
            } 

            else{
                printf("\n\n=======TIPO I=========\n");
                printf("opcode[%s]\t", instrucoesDecodificadas[i].opcode);
                printf("rs[%s]\t\t", instrucoesDecodificadas[i].rs);
                printf("rt[%s]\t\t", instrucoesDecodificadas[i].rt);
                printf("imm[%s]\t\t", instrucoesDecodificadas[i].imm);
                
                printf("\n======================\n");
            }
        }

        else if(memoria[i].uso == 1){
            printf("\n\nDado %d: [%s]",i+1, memoria[i].mem);
        }
            
    }
}


void imprimeEstatisticas(Memorias *memoria, int tamLinhas, type_instruc *instrucoesDecodificadas, int pc){
    if (memoria == NULL) {
                fprintf(stderr, "Falha ao obter instruções.\n");
            }
    int r=0, i=0, j=0, instAri=0, instLogic=0, instDesvio=0, instAcessoMem=0;
    for(int y=0;y<tamLinhas;y++){
        if(memoria[y].uso == 0){
            if (strncmp(memoria[y].mem, "0000", 4) == 0){ //compara os 4 primeiros numeros de memoria com "0000"
            r++;
            if ((strcmp(instrucoesDecodificadas[y].funct, "000")) || (strcmp(instrucoesDecodificadas[y].funct, "010") == 0))
                instAri++;
            else
                instLogic++;
            }
            else if (strncmp(memoria[y].mem, "0100", 4) == 0 || strncmp(memoria[y].mem, "1011", 4) == 0 || strncmp(memoria[y].mem, "1111", 4) == 0 || strncmp(memoria[y].mem, "0110", 4) == 0 || strncmp(memoria[y].mem, "1000", 4) == 0){
                i++;
                if (strncmp(memoria[y].mem, "0100", 4) == 0)
                    instAri++;
                else if ((strncmp(memoria[y].mem, "1011", 4) == 0) || strncmp(memoria[y].mem, "1111", 4) == 0)
                    instAcessoMem++;
                else if (strncmp(memoria[y].mem, "1000", 4) == 0)
                    instDesvio++;



            }  
            else if (strncmp(memoria[y].mem, "0010", 4) == 0){
                j++;
                instDesvio++;
            }
        }
        else{
            printf("\n");
        }
        
            
    }
    printf("O numero de instrucoes e de %d\n", tamLinhas);
    printf("PC com valor: %d\n", pc);
    printf("Observa-se: \n%d instrucoes do tipo R\n%d instrucoes do tipo I\n%d instrucoes do tipo J\n", r,i,j);
    printf("Onde %d instrucoes sao da classe Logica, %d Aritmetica, %d Desvio e %d Acesso a Memoria de Dados\n", instLogic, instAri, instDesvio, instAcessoMem);
}


void imprimeRegsAux(RegistradoresAux *aux){
    printf("\nRegistrador de Instrucao: %s", aux->registradorInst);
    printf("\nRegistrador de Dados da Memoria: %s", aux->registradorDados);
    printf("\nRegistrador A: %d", aux->registradorA);
    printf("\nRegistrador B: %d", aux->registradorB);
    printf("\nRegistrador ULA saida: %d\n\n", aux->registradorULA);
}