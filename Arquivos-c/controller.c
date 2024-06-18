#include "../Arquivos-h/controller.h"

int controller(int op, int *StateForBack, int NumeroLinhas, int *regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal, int ProxEtapa){
    int jump, RD, RT, i, a=0, verifica_fim = 0, immediate, dados, pc;
    char posicao[4];

    switch (op)
    {
    case 1:
        while (*program_counter <= NumeroLinhas)
        {        
            switch (ProxEtapa)
            {
            case 1://Etapa 1 -> Recebe Instrução e Incrementa program_counter
                printf("Program Counter -> %d\n", *program_counter);
                *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores
                //verifica se sera um instrucao ou dado
                if(md[*program_counter]->uso == 'i')
                    strcpy((*aux)->registradorInst, md[*program_counter]->mem);

                else{ //se for dado, incrementa pc e volta para o começo do while
                    increment_PC(program_counter, 1);
                    continue;
                }
                (*instrucoesDecodificadas)[*program_counter] = Memoria(*aux); //DECODIFICA A INSTRUCAO

                (*aux)->PC = *program_counter; // Declaro que o registrador auxiliar PC recebe o valor de program_counter, pois irei incrementar o program_counter nesta etapa
                increment_PC(program_counter, 1);

                increment_State(StateForBack, 1);

                controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 2);

                break;

            case 2://Etapa 2 -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
                *sinal = AddSinais(instrucoesDecodificadas[(*aux)->PC]);

                (*aux)->registradorA = (*sinal)->RS;
                (*aux)->registradorB = (*sinal)->RT;
                (*aux)->registradorULA = *program_counter + (*sinal)->imm;
     
                increment_State(StateForBack, 1); 
                controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 3);           
                break;
            
            case 3://Etapa 3 --> Executa tipo R e Addi, Calcula Endereço LW e SW, Desvia Jump e Beq            
                if ((*sinal)->tipo == 1)//verifica se é Jump
                {
                    jump = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                    (*program_counter) = jump;
                    a++;
    
                    increment_State(StateForBack, 1); 
                    (*aux) = NULL;;
                    controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                }
                
                else if ((*sinal)->tipo == 0 || (*sinal)->tipo == 2 || (*sinal)->tipo == 3 || (*sinal)->tipo == 4)
                //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                {
                    (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 

                    increment_State(StateForBack, 1); 
                    controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 4); 
                }
                
                else if ((*sinal)->tipo == 5)//verifica se é beq
                {
                    if ((*aux)->registradorA == (*aux)->registradorB){
                        *program_counter = (*aux)->registradorULA;

                        increment_State(StateForBack, 1); 
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                }
                break;
                                    
            case 4: // Etapa 4 -> Escreve em Regs para tipo R e Addi, Escreve em Memoria para SW, Lê Valor de Memória para LW              
                    if ((*sinal)->tipo == 3) // lw (load word)
                    {
                        // Carregar dado da memória
                        strcpy((*aux)->registradorDados, md[(*sinal)->imm]->mem + 8); //copio para o registrador de dados, o dado da memoria
                        //Agora sei qual o valor contido na posição 4 da memoria em decimal:
                        
   
                        increment_State(StateForBack, 1); 
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 5);
                    }
                    else if ((*sinal)->tipo == 4) // sw (store word)
                    {
                        // Armazenar dado na memória
                        decimalToBinary((*sinal)->RT, posicao);
                        int conteudo = retornoRegs(regs, posicao);
                        char conteudo_bin[9];
                        conteudo_bin[8]='\0';

                        if (conteudo > 127 || conteudo < -128){
                            fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                            if (conteudo > 127)
                                strcpy(conteudo_bin, "01111111"); //Escreve 127
                            else
                                strcpy(conteudo_bin, "10000000"); //Escreve -128
                                escreveDado(md, (*sinal)->imm, conteudo_bin);
                        }
                        decimalToBinary(conteudo, conteudo_bin);
                        escreveDado(md, (*sinal)->imm, conteudo_bin);

                        (*StateForBack)++;
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                    else if ((*sinal)->tipo == 0) // R-type
                    {
                        // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD); // para tipo R
                        decimalToBinary((*sinal)->RD, posicao);
                        escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                        (*StateForBack)++;
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                    else if ((*sinal)->tipo == 2) // addi
                    {
                        // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); // para addi
                        decimalToBinary((*sinal)->RT, posicao);
                        escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                        (*StateForBack)++;
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                    break;

                case 5: //Etapa 5 -> Escreve EM Regs para LW                
                    if ((*sinal)->tipo == 3){ // lw (load word)
                        decimalToBinary((*sinal)->RT, posicao);
                        dados = bin_to_decimal((*aux)->registradorDados);
                        escritaRegistradores(regs, dados, posicao); // Load: Reg[IR[20:16]] <= MDR
                         
                        increment_State(StateForBack, 1); 
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, program_counter, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                break;
                
                default:
                    printf ("%d", (*aux)->PC);
                break;
            }        
        }
        break;

    case 2:
        //Run by Step
        switch (ProxEtapa)
        {
        case 1:///Etapa 1 -> Recebe Instrução e Incrementa program_counter
            printf("instrucao: %s", md[*program_counter]->mem);
            printf("uso: %c", md[*program_counter]->uso);
            *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores
            //verifica se sera um instrucao ou dado
            
            if(md[*program_counter]->uso == 'i'){
                strcpy((*aux)->registradorInst, md[*program_counter]->mem);
                printf("Instrucao coletada com sucesso");
            }        
            else{ //se for dado, incrementa pc e quebra switch
                printf("Nao foi encontrado nenhuma instrucao\n");
                increment_PC(program_counter, 1);
                return 1;
            }

            (*instrucoesDecodificadas)[*program_counter] = Memoria(*aux); //DECODIFICA A INSTRUCAO
            printf("Instrucao decodificada %s\n", (*instrucoesDecodificadas)[*program_counter].opcode);
            (*aux)->PC = *program_counter; // Declaro que o registrador auxiliar PC recebe o valor de program_counter, pois irei incrementar o program_counter nesta etapa
            increment_PC(program_counter, 1);

            imprimeRegsAux(*aux);       
            increment_State(StateForBack, 1); 
            return 2;
            break;
            
        case 2://Etapa 2 -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares        
            *sinal = AddSinais(instrucoesDecodificadas[(*aux)->PC]);
            (*aux)->registradorA = (*sinal)->RS;
            (*aux)->registradorB = (*sinal)->RT;
            (*aux)->registradorULA = *program_counter + (*sinal)->imm;

            imprimeRegsAux(*aux);       
            increment_State(StateForBack, 1); 
            return 3;
            break;
                
        case 3: //Etapa 3 --> Executa tipo R e Addi, Calcula Endereço LW e SW, Desvia Jump e Beq        
            if ((*sinal)->tipo == 1)//verifica se é Jump
            {
                pc = (*aux)->PC;
                jump = ULA(instrucoesDecodificadas, &pc, md, regs, *aux);
                (*program_counter) = jump;
                a++;
                printf("\ntipo: %d\b", (*sinal)->tipo);
                printf("%d jump/loop concluido.\t\t", a);

                imprimeRegsAux(*aux);       
                increment_State(StateForBack, 1); 
                (*aux) = NULL;;
                return 1;
            }
            
            else if ((*sinal)->tipo == 0 || (*sinal)->tipo == 2 || (*sinal)->tipo == 3 || (*sinal)->tipo == 4)
            //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
            {
                pc = (*aux)->PC;
                (*aux)->registradorULA = ULA(instrucoesDecodificadas, &pc, md, regs, *aux); 
                imprimeRegsAux(*aux);
                increment_State(StateForBack, 1); 
                return 4;
            }
            
            else if ((*sinal)->tipo == 5)//verifica se é beq
            {
                if ((*aux)->registradorA == (*aux)->registradorB){
                    *program_counter = (*aux)->registradorULA;
                    imprimeRegsAux(*aux);
                    increment_State(StateForBack, 1); 
                    (*aux) = NULL;;
                    return 1;
                }
            }

            break;
                  
        case 4: // Etapa 4 -> Escreve em Regs para tipo R e Addi, Escreve em Memoria para SW, Lê Valor de Memória para LW
                if ((*sinal)->tipo == 3) // lw (load word)
                {
                    // Carregar dado da memória
                    strcpy((*aux)->registradorDados, md[(*sinal)->imm]->mem + 8); //copio para o registrador de dados, o dado da memoria
                    //Agora sei qual o valor contido na posição 4 da memoria em decimal:
                    
                    imprimeRegsAux(*aux);       
                    increment_State(StateForBack, 1); 
                    return 5;
            }
                else if ((*sinal)->tipo == 4) // sw (store word)
                {
                    // Armazenar dado na memória
                    decimalToBinary((*sinal)->RT, posicao);
                    int conteudo = retornoRegs(regs, posicao);
                    char conteudo_bin[9];
                    conteudo_bin[8]='\0';

                    if (conteudo > 127 || conteudo < -128){
                        fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                        if (conteudo > 127)
                            strcpy(conteudo_bin, "01111111"); //Escreve 127
                         else
                            strcpy(conteudo_bin, "10000000"); //Escreve -128
                            escreveDado(md, (*sinal)->imm, conteudo_bin);
                    }
                    decimalToBinary(conteudo, conteudo_bin);
                    escreveDado(md, (*sinal)->imm, conteudo_bin);

                    imprimeRegsAux(*aux);
                    (*StateForBack)++;
                    (*aux) = NULL;;
                    return 1;
                }
                else if ((*sinal)->tipo == 0) // R-type
                {
                    // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD); // para tipo R
                    decimalToBinary((*sinal)->RD, posicao);
                    escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                    imprimeRegsAux(*aux);
                    (*StateForBack)++;
                    (*aux) = NULL;;
                    return 1;
                }
                else if ((*sinal)->tipo == 2) // addi
                {
                    // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); // para addi
                    decimalToBinary((*sinal)->RT, posicao);
                    escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                    imprimeRegsAux(*aux);
                    (*StateForBack)++;
                    (*aux) = NULL;;                    
                    return 1;
                }
                break;

            case 5: //Etapa 5 -> Escreve EM Regs para LW            
                if ((*sinal)->tipo == 3){ // lw (load word)
                    decimalToBinary((*sinal)->RT, posicao);
                    dados = bin_to_decimal((*aux)->registradorDados);
                    escritaRegistradores(regs, dados, posicao); // Load: Reg[IR[20:16]] <= MDR

                    imprimeRegsAux(*aux);       
                    increment_State(StateForBack, 1); 

                    (*aux) = NULL;;
                    return 1;
                }
                break;
        }
    case 3:
        while (*StateForBack <= NumeroLinhas)
        {        
            switch (ProxEtapa)
            {
            case 1://Etapa 1 -> Recebe Instrução e Incrementa program_counter
                printf("Program Counter -> %d\n", *program_counter);
                *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores
                //verifica se sera um instrucao ou dado
                if(md[*program_counter]->uso == 'i')
                    strcpy((*aux)->registradorInst, md[*program_counter]->mem);

                else{ //se for dado, incrementa pc e volta para o começo do while
                    increment_PC(program_counter, 1);
                    continue;
                }
                (*instrucoesDecodificadas)[*program_counter] = Memoria(*aux); //DECODIFICA A INSTRUCAO

                (*aux)->PC = *program_counter; // Declaro que o registrador auxiliar PC recebe o valor de program_counter, pois irei incrementar o program_counter nesta etapa
                increment_PC(program_counter, 1);

                increment_State(StateForBack, 1);

                controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 2);

                break;

            case 2://Etapa 2 -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
                *sinal = AddSinais(instrucoesDecodificadas[(*aux)->PC]);

                (*aux)->registradorA = (*sinal)->RS;
                (*aux)->registradorB = (*sinal)->RT;
                (*aux)->registradorULA = *program_counter + (*sinal)->imm;
     
                increment_State(StateForBack, 1); 
                controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 3);           
                break;
            
            case 3://Etapa 3 --> Executa tipo R e Addi, Calcula Endereço LW e SW, Desvia Jump e Beq            
                if ((*sinal)->tipo == 1)//verifica se é Jump
                {
                    jump = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                    (*program_counter) = jump;
    
                    increment_State(StateForBack, 1); 
                    (*aux) = NULL;;
                    controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                }
                
                else if ((*sinal)->tipo == 0 || (*sinal)->tipo == 2 || (*sinal)->tipo == 3 || (*sinal)->tipo == 4)
                //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                {
                    (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 

                    increment_State(StateForBack, 1); 
                    controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 4); 
                }
                
                else if ((*sinal)->tipo == 5)//verifica se é beq
                {
                    if ((*aux)->registradorA == (*aux)->registradorB){
                        *program_counter = (*aux)->registradorULA;

                        increment_State(StateForBack, 1); 
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                }
                break;
                                    
            case 4: // Etapa 4 -> Escreve em Regs para tipo R e Addi, Escreve em Memoria para SW, Lê Valor de Memória para LW              
                    if ((*sinal)->tipo == 3) // lw (load word)
                    {
                        // Carregar dado da memória
                        strcpy((*aux)->registradorDados, md[(*sinal)->imm]->mem + 8); //copio para o registrador de dados, o dado da memoria
                        //Agora sei qual o valor contido na posição 4 da memoria em decimal:
                        
   
                        increment_State(StateForBack, 1); 
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 5);
                    }
                    else if ((*sinal)->tipo == 4) // sw (store word)
                    {
                        // Armazenar dado na memória
                        decimalToBinary((*sinal)->RT, posicao);
                        int conteudo = retornoRegs(regs, posicao);
                        char conteudo_bin[9];
                        conteudo_bin[8]='\0';

                        if (conteudo > 127 || conteudo < -128){
                            fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                            if (conteudo > 127)
                                strcpy(conteudo_bin, "01111111"); //Escreve 127
                            else
                                strcpy(conteudo_bin, "10000000"); //Escreve -128
                                escreveDado(md, (*sinal)->imm, conteudo_bin);
                        }
                        decimalToBinary(conteudo, conteudo_bin);
                        escreveDado(md, (*sinal)->imm, conteudo_bin);

                        (*StateForBack)++;
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                    else if ((*sinal)->tipo == 0) // R-type
                    {
                        // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD); // para tipo R
                        decimalToBinary((*sinal)->RD, posicao);
                        escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                        (*StateForBack)++;
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                    else if ((*sinal)->tipo == 2) // addi
                    {
                        // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); // para addi
                        decimalToBinary((*sinal)->RT, posicao);
                        escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                        (*StateForBack)++;
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                    break;

                case 5: //Etapa 5 -> Escreve EM Regs para LW                
                    if ((*sinal)->tipo == 3){ // lw (load word)
                        decimalToBinary((*sinal)->RT, posicao);
                        dados = bin_to_decimal((*aux)->registradorDados);
                        escritaRegistradores(regs, dados, posicao); // Load: Reg[IR[20:16]] <= MDR
                         
                        increment_State(StateForBack, 1); 
                        (*aux) = NULL;;
                        controller(1, StateForBack, NumeroLinhas, regs, md, program_counter, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                break;
                
                default:
                    printf ("%d", (*aux)->PC);
                break;
            }        
        }
        return ProxEtapa;
        break;

    }
}

/*
void backstep(int *StateForBack, int tamLinhas, int *regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal)
{
    int jump, RD, RT, i, a=0, auxiliar;
    for (i = 0; i<8; i++){
        regs[i]=0;
    }
    printf("%d",*StateForBack);
    auxiliar = *StateForBack;
    *StateForBack = -1;
    increment_PC(program_counter, 0);

    controller(1, StateForBack, auxiliar, regs, md, program_counter, instrucoesDecodificadas, aux, sinal, 1);

    imprimeRegsAux(*aux);
}
*/