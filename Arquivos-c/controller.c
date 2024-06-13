
#include "../Arquivos-h/controller.h"

int controller(int op, int *StateForBack, Memorias **memoriaInst, int NumeroLinhas, int **regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal, int ProxEtapa){
    int jump, RD, RT, i, a=0, *PC, verifica_fim = 0, immediate;

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
            if ((*sinal)->tipo == 0 || (*sinal)->tipo == 3 || (*sinal)->tipo == 4){
                if ((*sinal)->tipo == 3) {
                    // para lw fazer a leitura do dado
                    immediate = bin_to_decimal((*sinal)->imm);
                    strcpy((*aux)->registradorDados, *md[immediate]->dados); //copio para o registrador de dados, o dado da memoria
                    //dados serão transferidos na próxima etapa (etapa 5)
                } else if ((*sinal)->tipo == 4) {
                    //para SW escrever na memória o valor
                    int conteudo = retornoRegs(regs, (*sinal)->RT);
                    char conteudo_bin[9];
                    conteudo_bin[8]='\0';

                    if (conteudo > 127 || conteudo < -128){
                        fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                        if (conteudo > 127)
                            strcpy(conteudo_bin, "01111111"); //Escreve 127
                        else
                            strcpy(conteudo_bin, "10000000"); //Escreve -128
                        escreveDado(*md, immediate, conteudo_bin);
                        return -1;
                    }
                    decimalToBinary(conteudo, conteudo_bin);
                    escreveDado(*md, &immediate, conteudo_bin);
                } else if ((*sinal)->tipo == 0) {
                    // escrita nos registradores para instruções do tipo R
                    escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD); // para tipo R
                }
            }

            //Etapa 5
            if ((*sinal)->tipo == 3) {
                // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT);
                int rt = (*sinal)->RT;
                escritaRegistradores(regs, (*aux)->registradorDados, rt); // Load: Reg[IR[20:16]] <= MDR
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
                if ((*sinal)->tipo == 3) // lw (load word)
                {
                    // Carregar dado da memória
                        int immediate = bin_to_decimal((*sinal)->imm);
                        strcpy((*aux)->registradorDados, *md[immediate]->dados); //copio para o registrador de dados, o dado da memoria
                        //Agora sei qual o valor contido na posição 4 da memoria em decimal:
                        
                        imprimeRegsAux(aux);       
                        (*StateForBack)++;  
                        return ProxEtapa++;
                }
                else if ((*sinal)->tipo == 4) // sw (store word)
                {
                    // Armazenar dado na memória
                    int immediate = bin_to_decimal((*sinal)->imm);
                    int conteudo = retornoRegs(regs, (*sinal)->RT);
                    char conteudo_bin[9];
                    conteudo_bin[8]='\0';

                    if (conteudo > 127 || conteudo < -128){
                        fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                        if (conteudo > 127)
                             strcpy(conteudo_bin, "01111111"); //Escreve 127
                         else
                             strcpy(conteudo_bin, "10000000"); //Escreve -128
                            escreveDado(md, &immediate, conteudo_bin);
                         return -1;
                        }
                        decimalToBinary(conteudo, conteudo_bin);
                        escreveDado(md, &immediate, conteudo_bin);

                        imprimeRegsAux(aux);
                        (*StateForBack)++;
                        return ProxEtapa++;
                }
                else if ((*sinal)->tipo == 0) // R-type
                {
                    // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD); // para tipo R
                    escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RD);

                    imprimeRegsAux(aux);
                    (*StateForBack)++;
                    return ProxEtapa++;
                }
                else if ((*sinal)->tipo == 2) // addi
                {
                    // escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT); // para addi
                    escritaRegistradores(regs, (*aux)->registradorULA, (*sinal)->RT);

                    imprimeRegsAux(aux);
                    (*StateForBack)++;
                    return ProxEtapa++;
                }
            }

            if (ProxEtapa == 5) //Etapa 5 -> Escreve EM Regs para LW
            {
                if ((*sinal)->tipo == 3){ // lw (load word)
                    int rt = (*sinal)->RT;
                    escritaRegistradores(regs, (*aux)->registradorDados, rt); // Load: Reg[IR[20:16]] <= MDR
                
                    imprimeRegsAux(aux);       
                    (*StateForBack)++;  
                    return 1;
                }
                break;
            }
        }
}

int controller(int op, int *StateForBack, Memorias **memoriaInst, int tamLinhas, int **regs, Memorias **md, int *program_counter, type_instruc **instrucoesDecodificadas, RegistradoresAux **aux, Sinais **sinal, int ProxEtapa)
{
    *aux = inicializaRegsAux(); //reinicializa-ra os registradores para armazenar novos valores

    int jump, RD, RT, i, a=0;
    for (i = 0; i<8; i++){
        (*regs)[i]=0;
    }

    for (*program_counter = 0; *program_counter < (*StateForBack - 1); increment_PC(program_counter, 1)){
        //A fazer
    }

    (*StateForBack)--;
    imprimeRegsAux(aux);
}
