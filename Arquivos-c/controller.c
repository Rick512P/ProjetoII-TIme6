
#include "../Arquivos-h/controller.h"

int controller(int op, int *StateForBack, Memorias **memoriaInst, int NumeroLinhas, int **regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal, int ProxEtapa){
    int jump, RD, RT, i, a=0, *PC, verifica_fim = 0;

    /*for(int j=0;j<NumeroLinhas;j++){
        strcpy(aux->registradorInst, memoriaInst[j]->instruc);
        (*instrucoesDecodificadas)[j] = memInstruc(aux);
    }*/
    

    switch (op)
    {
    case 1:
        while (program_counter < NumeroLinhas){
            //Etapa 1 -> Recebe Instrução e Incrementa program_counter
            *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores
            strcpy((*aux)->registradorInst, memoriaInst[*program_counter]->instruc);

            (*instrucoesDecodificadas)[*program_counter] = Memoria(*aux); //DECODIFICA A INSTRUCAO

            PC = program_counter; // Declaro que o registrador auxiliar PC recebe o valor de program_counter, pois irei incrementar o program_counter nesta etapa
            increment_PC(program_counter, 0); 


            //Etapa 2 -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
            *sinal = AddSinais(instrucoesDecodificadas[*PC]);

            (*aux)->registradorA = (*sinal)->RS;
            (*aux)->registradorB = (*sinal)->RT;
            (*aux)->registradorULA = program_counter + (*sinal)->imm;


            //Etapa 3 -->

            if ((*sinal)->tipo == 1)//verifica se é Jump
            {
                jump = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                (*program_counter) = jump;
                a++;
                printf("%d jump/loop concluido.\t\t", a);
            }
            
            else if ((*sinal)->tipo == 0 || (*sinal)->tipo == 2 || (*sinal)->tipo == 3 || (*sinal)->tipo == 4)
            //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
            {
                (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 
            }
            
            else if ((*sinal)->tipo == 5)//verifica se é beq
            {
                if ((*aux)->registradorA == (*aux)->registradorB)
                    program_counter = (*aux)->registradorULA;
            }
            
            //Etapa 4
            if ((*sinal)->tipo == 0 || (*sinal)->tipo == 3 || (*sinal)->tipo == 4)
            {
                // para lw fazer a leitura do dado
                /*
                int immediate, dados;
                immediate = bin_to_decimal((*instrucoesDecodificadas)[*contador].imm);
                //Agora sei qual a posicao Immediate em decimal:
                strcpy(aux->registradorDados, memoria[immediate]->dados); //copio para o registrador de dados, o dado da memoria
                dados = bin_to_decimal(memoria[immediate]->dados);
                //Agora sei qual o valor contido na posição 4 da memoria em decimal:
                return dados;
                */
                
                //para SW escrever na memória o valor
                /*
                    // Para escrever valor recebido de retornoRegs(regs, (*sinal)->RT); no endereço dado por SW que está salvo em (*aux)->registradorULA
                    // deve ser feito no Etapa 4
                    int conteudo = retornoRegs(regs, (*sinal)->RT);
                    char conteudo_bin[9];
                    conteudo_bin[8]='\0';

                    if (conteudo > 127 || conteudo < -128){
                        fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                        if (conteudo > 127)
                            strcpy(conteudo_bin, "01111111"); //Escreve 32
                        else
                            strcpy(conteudo_bin, "10000001"); //Escreve -32
                        escreveDado(memoria, &imm, conteudo_bin);
                        return -1;
                    }
                    decimalToBinary(aux->registradorB, conteudo_bin);
                    escreveDado(memoria, &imm, conteudo_bin);
                    */
                
                // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD); // para tipo R
                // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); // para addi
            }

            //Etapa 5

            if ((*sinal)->tipo == 3)
            {
                // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); 
            }
        
            (*StateForBack)++;  
            imprimeRegsAux(aux);       
        }        
        break;

    case 2:
        //Run by Step
        if (ProxEtapa == 1)//Etapa 1 -> Recebe Instrução e Incrementa program_counter
        {
            *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores
            strcpy((*aux)->registradorInst, memoriaInst[*program_counter]->instruc);

            (*instrucoesDecodificadas)[*program_counter] = Memoria(*aux); //DECODIFICA A INSTRUCAO

            PC = program_counter; // Declaro que o registrador auxiliar PC recebe o valor de program_counter, pois irei incrementar o program_counter nesta etapa
            increment_PC(program_counter, 0);

            imprimeRegsAux(aux);       
            (*StateForBack)++;  
            return ProxEtapa++;
        }

        else if (ProxEtapa == 2)//Etapa 2 -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
        {
            *sinal = AddSinais(instrucoesDecodificadas[*PC]);

            (*aux)->registradorA = (*sinal)->RS;
            (*aux)->registradorB = (*sinal)->RT;
            (*aux)->registradorULA = program_counter + (*sinal)->imm;

            imprimeRegsAux(aux);       
            (*StateForBack)++;  
            return ProxEtapa++;
        }
        
        if (ProxEtapa == 3)//Etapa 3 --> Executa tipo R e Addi, Calcula Endereço LW e SW, Desvia Jump e Beq
        {
            if ((*sinal)->tipo == 1)//verifica se é Jump
            {
                jump = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux);
                (*program_counter) = jump;
                a++;
                printf("%d jump/loop concluido.\t\t", a);

                imprimeRegsAux(aux);       
                (*StateForBack)++;  
                return 1;
            }
            
            else if ((*sinal)->tipo == 0 || (*sinal)->tipo == 2 || (*sinal)->tipo == 3 || (*sinal)->tipo == 4)
            //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
            {
                (*aux)->registradorULA = ULA(instrucoesDecodificadas, program_counter, md, regs, *aux); 
            }
            
            else if ((*sinal)->tipo == 5)//verifica se é beq
            {
                if ((*aux)->registradorA == (*aux)->registradorB){
                    program_counter = (*aux)->registradorULA;
                    (*StateForBack)++;  
                    return 1;
                }
            }

            imprimeRegsAux(aux);       
            (*StateForBack)++;  
            return ProxEtapa++;
        }
                  
        if (ProxEtapa == 4) // Etapa 4 -> Escreve em Regs para tipo R e Addi, Escreve em Memoria para SW, Lê Valor de Memória para LW
            {
                if ((*sinal)->tipo == 3)
                {
                    // para lw fazer a leitura do dado
                    /*
                        int immediate, dados;
                        immediate = bin_to_decimal((*instrucoesDecodificadas)[*contador].imm);
                        //Agora sei qual a posicao Immediate em decimal:
                        strcpy(aux->registradorDados, memoria[immediate]->dados); //copio para o registrador de dados, o dado da memoria
                        dados = bin_to_decimal(memoria[immediate]->dados);
                        //Agora sei qual o valor contido na posição 4 da memoria em decimal:
                        (*aux)->registradorULA = dados;
                        
                        imprimeRegsAux(aux);       
                        (*StateForBack)++;  
                        return ProxEtapa++;
                    */
                }
                else if ((*sinal)->tipo == 4)
                {
                    //para SW escrever na memória o valor
                    /*
                        // Para escrever valor recebido de retornoRegs(regs, (*sinal)->RT); no endereço dado por SW que está salvo em (*aux)->registradorULA
                        // deve ser feito no Etapa 4
                        int conteudo = retornoRegs(regs, (*sinal)->RT);
                        char conteudo_bin[9];
                        conteudo_bin[8]='\0';

                        if (conteudo > 127 || conteudo < -128){
                            fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                            if (conteudo > 127)
                                strcpy(conteudo_bin, "01111111"); //Escreve 32
                            else
                                strcpy(conteudo_bin, "10000001"); //Escreve -32
                            escreveDado(memoria, &imm, conteudo_bin);
                            return -1;
                        }
                        decimalToBinary(aux->registradorB, conteudo_bin);
                        escreveDado(memoria, &imm, conteudo_bin);
                    */
                }
                else if ((*sinal)->tipo == 0)
                {
                    // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD); // para tipo R
                }
                else if ((*sinal)->tipo == 2)
                {
                    // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); // para addi
                }

                imprimeRegsAux(aux);       
                (*StateForBack)++;  
                return ProxEtapa++;
            }

            if (ProxEtapa) //Etapa 5 -> Escreve EM Regs para LW
            {
                // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); 
                imprimeRegsAux(aux);       
                (*StateForBack)++;  
                return 1;
            }
        break;
    }
}

void backstep(int *StateForBack, Memorias **memoriaInst, int NumeroLinhas, int **regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal){
    *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores

    int jump, RD, RT, i, a=0;
    for (i = 0; i<8; i++){
        (*regs)[i]=0;
    }

    for (*program_counter = 0; *program_counter < (*StateForBack - 1); increment_PC(program_counter, 1)){

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
    (*StateForBack)--;
    imprimeRegsAux(aux); 
    printf("Valor de StateForBack APOS BACK: %d\n", *StateForBack);
}
