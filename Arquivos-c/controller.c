
#include "../Arquivos-h/controller.h"

int controller(int op, int *state, instrucao **memoriaInst, int tamLinhas, int **regs, MemoriaDados **md, int *program_counter, type_instruc **instrucoesDecodificadas){
    int jump, RD, RT, i, a=0;

    for(int j=0;j<tamLinhas;j++){
        (*instrucoesDecodificadas)[j] = memInstruc(j, memoriaInst, &tamLinhas);
    }
    
    switch (op)
    {
    case 1:

        for ((*program_counter) = 0; (*program_counter) < tamLinhas; increment_PC(program_counter, 1))
        {   
            if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"0000")) == 0) // ADD/SUB/OR/AND
            {
                RD = ULA(instrucoesDecodificadas, program_counter, md, regs);
                escritaRegistradores(regs, RD, (*instrucoesDecodificadas)[*program_counter].rd);
            }
            else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"0100")) == 0){ //ADDI
                RT = ULA(instrucoesDecodificadas, program_counter, md, regs); 
                escritaRegistradores(regs, RT, (*instrucoesDecodificadas)[*program_counter].rt);
            }
            else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"1011")) == 0){  //LW
                RT = ULA(instrucoesDecodificadas, program_counter, md, regs);
                escritaRegistradores(regs, RT, (*instrucoesDecodificadas)[*program_counter].rt);
            }
            else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"1111")) == 0){ //SW
                ULA(instrucoesDecodificadas, program_counter, md, regs); 
            }
            else if (strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"1000") == 0) //BEQ
            {
                (*program_counter) = ULA(instrucoesDecodificadas, program_counter, md, regs);
            }            
            else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"0010")) == 0){ //JUMP

                jump = ULA(instrucoesDecodificadas, program_counter, md, regs);
                (*program_counter) = jump;
                a++;
                printf("%d jump/loop concluido.\t\t", a);
            }
            (*state)++;            
        }
        break;

    case 2:
        //Run by Step
            printf("Instrucao %d\n", *program_counter);
            increment_PC(program_counter, 0);  //FUNÇÃO QUE ESTA NO PC.C 
            
            if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"0000")) == 0)
            {
                
                RD = ULA(instrucoesDecodificadas, program_counter, md, regs);
                escritaRegistradores(regs, RD, (*instrucoesDecodificadas)[*program_counter].rd);

            }
            else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"0100")) == 0){
                
                RT = ULA(instrucoesDecodificadas, program_counter, md, regs);
                escritaRegistradores(regs, RT, (*instrucoesDecodificadas)[*program_counter].rt);

            }
            else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"1011")) == 0){

                RT = ULA(instrucoesDecodificadas, program_counter, md, regs);
                escritaRegistradores(regs, RT, (*instrucoesDecodificadas)[*program_counter].rt);

            }
            else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"1111")) == 0){

                ULA(instrucoesDecodificadas, program_counter, md, regs);

            }
            else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"0010")) == 0){
                (*program_counter) = ULA(instrucoesDecodificadas, program_counter, md, regs);

            }
            else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"1000")) == 0){ //beq
                (*program_counter) = ULA(instrucoesDecodificadas, program_counter, md, regs);
            }
            (*state)++;
            printf("Valor de state APOS STEP: %d", *state);
            increment_PC(program_counter, 1);
        break;
    }

    return 0;
}

void backstep(int *state, instrucao **memoriaInst, int tamLinhas, int **regs, MemoriaDados **md, int *program_counter, type_instruc **instrucoesDecodificadas){
    int jump, RD, RT, i, a=0;
    for (i = 0; i<8; i++){
        (*regs)[i]=0;
    }

    for (*program_counter = 0; *program_counter < (*state - 1); increment_PC(program_counter, 1)){ 
        if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"0000")) == 0) // ADD/SUB/OR/AND
        {
            RD = ULA(instrucoesDecodificadas, program_counter, md, regs);
            escritaRegistradores(regs, RD, (*instrucoesDecodificadas)[*program_counter].rd);
        }
        else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"0100")) == 0){ //ADDI
            RT = ULA(instrucoesDecodificadas, program_counter, md, regs); 
            escritaRegistradores(regs, RT, (*instrucoesDecodificadas)[*program_counter].rt);
        }
        else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"1011")) == 0){  //LW
            RT = ULA(instrucoesDecodificadas, program_counter, md, regs);
            escritaRegistradores(regs, RT, (*instrucoesDecodificadas)[*program_counter].rt);
        }
        else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"1111")) == 0){ //SW
            ULA(instrucoesDecodificadas, program_counter, md, regs); 
        }
        else if (strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"1000") == 0) //BEQ
        {
            (*program_counter) = ULA(instrucoesDecodificadas, program_counter, md, regs);
        }            
        else if ((strcmp((*instrucoesDecodificadas)[*program_counter].opcode,"0010")) == 0){ //JUMP
            jump = ULA(instrucoesDecodificadas, program_counter, md, regs);
            (*program_counter) = jump;
            a++;
            printf("%d jump/loop concluido.\t\t", a);
        }       
    }
    (*state)--;
    printf("Valor de state APOS BACK: %d\n", *state);
}
