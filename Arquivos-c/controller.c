#include "../Arquivos-h/controller.h"

int controller(int op, int *StateForBack, int NumeroLinhas, int *regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal, int ProxEtapa){
    int jump, RD, RT, i, a=0, verifica_fim = 0, immediate, dados;
    char posicao[4];

    switch (op)
    {
    case 1:
        while (*program_counter != NumeroLinhas)
        {        
            if (ProxEtapa == 1)//Etapa 1 -> Recebe Instrução e Incrementa program_counter
            {
                printf("\npc: %d\n", *program_counter);
                free(*aux);
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

                imprimeRegsAux(*aux);       
                (*StateForBack)++;  
                controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 2);
            }

            else if (ProxEtapa == 2)//Etapa 2 -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
            {
                *sinal = AddSinais(instrucoesDecodificadas[(*aux)->PC]);

                (*aux)->registradorA = (*sinal)->RS;
                (*aux)->registradorB = (*sinal)->RT;
                (*aux)->registradorULA = *program_counter + (*sinal)->imm;

                imprimeRegsAux(*aux);       
                (*StateForBack)++;  
                controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 3);           
            }
            
            if (ProxEtapa == 3)//Etapa 3 --> Executa tipo R e Addi, Calcula Endereço LW e SW, Desvia Jump e Beq
            {
                if ((*sinal)->tipo == 1)//verifica se é Jump
                {
                    jump = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                    (*program_counter) = jump;
                    a++;
                    printf("%d jump/loop concluido.\t\t", a);

                    imprimeRegsAux(*aux);       
                    (*StateForBack)++;  
                    controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                }
                
                else if ((*sinal)->tipo == 0 || (*sinal)->tipo == 2 || (*sinal)->tipo == 3 || (*sinal)->tipo == 4)
                //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                {
                    (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 
                    imprimeRegsAux(*aux);
                    (*StateForBack)++;  
                    controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 4); 
                }
                
                else if ((*sinal)->tipo == 5)//verifica se é beq
                {
                    if ((*aux)->registradorA == (*aux)->registradorB){
                        *program_counter = (*aux)->registradorULA;
                        imprimeRegsAux(*aux);
                        (*StateForBack)++;  
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                }
            }
                    
            if (ProxEtapa == 4) // Etapa 4 -> Escreve em Regs para tipo R e Addi, Escreve em Memoria para SW, Lê Valor de Memória para LW
                {
                    if ((*sinal)->tipo == 3) // lw (load word)
                    {
                        // Carregar dado da memória
                            strcpy((*aux)->registradorDados, md[(*sinal)->imm]->mem + 8); //copio para o registrador de dados, o dado da memoria
                            //Agora sei qual o valor contido na posição 4 da memoria em decimal:
                            
                            imprimeRegsAux(*aux);       
                            (*StateForBack)++;  
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

                            imprimeRegsAux(*aux);
                            (*StateForBack)++;
                            controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                    else if ((*sinal)->tipo == 0) // R-type
                    {
                        // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD); // para tipo R
                        decimalToBinary((*sinal)->RD, posicao);
                        escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                        imprimeRegsAux(*aux);
                        (*StateForBack)++;
                        return 1;
                    }
                    else if ((*sinal)->tipo == 2) // addi
                    {
                        // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); // para addi
                        decimalToBinary((*sinal)->RT, posicao);
                        escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                        imprimeRegsAux(*aux);
                        (*StateForBack)++;
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                }

                if (ProxEtapa == 5) //Etapa 5 -> Escreve EM Regs para LW
                {
                    if ((*sinal)->tipo == 3){ // lw (load word)
                        decimalToBinary((*sinal)->RT, posicao);
                        dados = bin_to_decimal((*aux)->registradorDados);
                        escritaRegistradores(regs, dados, posicao); // Load: Reg[IR[20:16]] <= MDR
                    
                        imprimeRegsAux(*aux);       
                        (*StateForBack)++;  
                        controller(1, StateForBack, NumeroLinhas, regs, md, &(*aux)->PC, instrucoesDecodificadas, aux, sinal, 1); 
                    }
                    break;
                }      
        }
        break;

    case 2:
        //Run by Step
        if (ProxEtapa == 1)//Etapa 1 -> Recebe Instrução e Incrementa program_counter
        {
            free(*aux);
            *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores
            //verifica se sera um instrucao ou dado
            if(md[*program_counter]->uso == 'i')
                strcpy((*aux)->registradorInst, md[*program_counter]->mem);
                    
            else{ //se for dado, incrementa pc e quebra switch
                increment_PC(program_counter, 1);
                break;
                }

            (*instrucoesDecodificadas)[*program_counter] = Memoria(*aux); //DECODIFICA A INSTRUCAO

            (*aux)->PC = *program_counter; // Declaro que o registrador auxiliar PC recebe o valor de program_counter, pois irei incrementar o program_counter nesta etapa
            increment_PC(program_counter, 1);

            imprimeRegsAux(*aux);       
            (*StateForBack)++;  
            return 2;
        }

        else if (ProxEtapa == 2)//Etapa 2 -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
        {
            *sinal = AddSinais(instrucoesDecodificadas[(*aux)->PC]);

            (*aux)->registradorA = (*sinal)->RS;
            (*aux)->registradorB = (*sinal)->RT;
            (*aux)->registradorULA = *program_counter + (*sinal)->imm;

            imprimeRegsAux(*aux);       
            (*StateForBack)++;  
            return 3;
        }
        
        if (ProxEtapa == 3)//Etapa 3 --> Executa tipo R e Addi, Calcula Endereço LW e SW, Desvia Jump e Beq
        {
            if ((*sinal)->tipo == 1)//verifica se é Jump
            {
                jump = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                (*program_counter) = jump;
                a++;
                printf("%d jump/loop concluido.\t\t", a);

                imprimeRegsAux(*aux);       
                (*StateForBack)++;  
                return 1;
            }
            
            else if ((*sinal)->tipo == 0 || (*sinal)->tipo == 2 || (*sinal)->tipo == 3 || (*sinal)->tipo == 4)
            //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
            {
                (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 
                imprimeRegsAux(*aux);
                (*StateForBack)++;  
                return 4;
            }
            
            else if ((*sinal)->tipo == 5)//verifica se é beq
            {
                if ((*aux)->registradorA == (*aux)->registradorB){
                    *program_counter = (*aux)->registradorULA;
                    imprimeRegsAux(*aux);
                    (*StateForBack)++;  
                    return 1;
                }
            }

            
        }
                  
        if (ProxEtapa == 4) // Etapa 4 -> Escreve em Regs para tipo R e Addi, Escreve em Memoria para SW, Lê Valor de Memória para LW
            {
                if ((*sinal)->tipo == 3) // lw (load word)
                {
                    // Carregar dado da memória
                        strcpy((*aux)->registradorDados, md[(*sinal)->imm]->mem + 8); //copio para o registrador de dados, o dado da memoria
                        //Agora sei qual o valor contido na posição 4 da memoria em decimal:
                        
                        imprimeRegsAux(*aux);       
                        (*StateForBack)++;  
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
                        return 1;
                }
                else if ((*sinal)->tipo == 0) // R-type
                {
                    // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD); // para tipo R
                    decimalToBinary((*sinal)->RD, posicao);
                    escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                    imprimeRegsAux(*aux);
                    (*StateForBack)++;
                    return 1;
                }
                else if ((*sinal)->tipo == 2) // addi
                {
                    // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); // para addi
                    decimalToBinary((*sinal)->RT, posicao);
                    escritaRegistradores(regs, (*aux)->registradorULA, posicao);

                    imprimeRegsAux(*aux);
                    (*StateForBack)++;
                    return 1;
                }
            }

            if (ProxEtapa == 5) //Etapa 5 -> Escreve EM Regs para LW
            {
                if ((*sinal)->tipo == 3){ // lw (load word)
                    decimalToBinary((*sinal)->RT, posicao);
                    dados = bin_to_decimal((*aux)->registradorDados);
                    escritaRegistradores(regs, dados, posicao); // Load: Reg[IR[20:16]] <= MDR
                
                    imprimeRegsAux(*aux);       
                    (*StateForBack)++;  
                    return 1;
                }
                break;
            }
        }
}


void backstep(int *StateForBack, int tamLinhas, int *regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal)
{
    int jump, RD, RT, i, a=0;
    for (i = 0; i<8; i++){
        regs[i]=0;
    }

    controller(1, StateForBack, *StateForBack, regs, md, 0, instrucoesDecodificadas, aux, sinal, 1);

    (*StateForBack)--;
    imprimeRegsAux(*aux);
}