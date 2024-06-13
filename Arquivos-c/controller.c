
#include "../Arquivos-h/controller.h"

int controller(int op, int *state, Memorias **memoriaInst, int tamLinhas, int **regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal){
    int jump, RD, RT, i, a=0;

    /*for(int j=0;j<tamLinhas;j++){
        strcpy(aux->registradorInst, memoriaInst[j]->instruc);
        (*instrucoesDecodificadas)[j] = memInstruc(aux);
    }*/
    

    switch (op)
    {
    case 1:

        for ((*program_counter) = 0; (*program_counter) < tamLinhas; increment_PC(program_counter, 1))
        {   
            
            *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores
            strcpy((*aux)->registradorInst, memoriaInst[*program_counter]->instruc);
            (*instrucoesDecodificadas)[*program_counter] = Memoria(*aux); //DECODIFICA A INSTRUCAO
            *sinal = AddSinais(instrucoesDecodificadas[*program_counter]);

            if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"0000")) == 0) // ADD/SUB/OR/AND
            {
                (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                escritaRegistradores(regs, (*aux)->registradorULA, instrucoesDecodificadas[*program_counter]->rd);
            }
            else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"0100")) == 0){ //ADDI
                (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 
                escritaRegistradores(regs, (*aux)->registradorULA, instrucoesDecodificadas[*program_counter]->rt);
            }
            else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"1011")) == 0){  //LW
                //ULA GERARA SINAL PARA ACESSAR A MEMORIA E ENTAO PEGAR O DADO
                (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 
                escritaRegistradores(regs, (*aux)->registradorULA, instrucoesDecodificadas[*program_counter]->rt);
            }
            else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"1111")) == 0){ //SW
                ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 
            }
            else if (strcmp(instrucoesDecodificadas[*program_counter]->opcode,"1000") == 0) //BEQ
            {
                (*program_counter) = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
            }            
            else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"0010")) == 0){ //JUMP

                jump = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                (*program_counter) = jump;
                a++;
                printf("%d jump/loop concluido.\t\t", a);
            }
            (*state)++;  
            imprimeRegsAux(aux);        
        }
        break;

    case 2:
        //Run by Step
            *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores
            printf("Instrucao %d\n", *program_counter);
            increment_PC(program_counter, 0);  //FUNÇÃO QUE ESTA NO PC.C 

            strcpy((*aux)->registradorInst, memoriaInst[*program_counter]->instruc);
            (*instrucoesDecodificadas)[*program_counter] = Memoria(*aux);
            *sinal = AddSinais(instrucoesDecodificadas[*program_counter]);
            
            if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"0000")) == 0)
            {
                
                (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                escritaRegistradores(regs, (*aux)->registradorULA, instrucoesDecodificadas[*program_counter]->rd);

            }
            else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"0100")) == 0){
                
                (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                escritaRegistradores(regs, (*aux)->registradorULA, instrucoesDecodificadas[*program_counter]->rt);

            }
            else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"1011")) == 0){

                (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                escritaRegistradores(regs, (*aux)->registradorULA, instrucoesDecodificadas[*program_counter]->rt);

            }
            else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"1111")) == 0){

                ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);

            }
            else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"0010")) == 0){
                (*program_counter) = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);

            }
            else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"1000")) == 0){ //beq
                (*program_counter) = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
            }
            (*state)++;
            printf("Valor de state APOS STEP: %d", *state);
            increment_PC(program_counter, 1);
            imprimeRegsAux(aux); 
        break;
    }

    return 0;
}

void backstep(int *state, Memorias **memoriaInst, int tamLinhas, int **regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal){
    *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores

    int jump, RD, RT, i, a=0;
    for (i = 0; i<8; i++){
        (*regs)[i]=0;
    }

    for (*program_counter = 0; *program_counter < (*state - 1); increment_PC(program_counter, 1)){

        strcpy((*aux)->registradorInst, memoriaInst[*program_counter]->instruc);
        (*instrucoesDecodificadas)[*program_counter] = Memoria(*aux); 
        *sinal = AddSinais(instrucoesDecodificadas[*program_counter]);

        if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"0000")) == 0) // ADD/SUB/OR/AND
        {
            (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
            escritaRegistradores(regs, (*aux)->registradorULA, instrucoesDecodificadas[*program_counter]->rd);
        }
        else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"0100")) == 0){ //ADDI
            (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 
            escritaRegistradores(regs, (*aux)->registradorULA, instrucoesDecodificadas[*program_counter]->rt);
        }
        else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"1011")) == 0){  //LW
            (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
            escritaRegistradores(regs, (*aux)->registradorULA, instrucoesDecodificadas[*program_counter]->rt);
        }
        else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"1111")) == 0){ //SW
            ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 
        }
        else if (strcmp(instrucoesDecodificadas[*program_counter]->opcode,"1000") == 0) //BEQ
        {
            (*program_counter) = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
        }            
        else if ((strcmp(instrucoesDecodificadas[*program_counter]->opcode,"0010")) == 0){ //JUMP
            jump = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
            (*program_counter) = jump;
            a++;
            printf("%d jump/loop concluido.\t\t", a);
        }       
    }
    (*state)--;
    imprimeRegsAux(aux); 
    printf("Valor de state APOS BACK: %d\n", *state);
}
