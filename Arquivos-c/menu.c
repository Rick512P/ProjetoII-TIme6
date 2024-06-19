#include "../Arquivos-h/menu.h"

int main(){
    menu();
    return 0;
}

int menu(){
    Assembly *AssemblyInst;
    Memorias *memorias = NULL;
    RegistradoresAux *aux = malloc(256 * sizeof(RegistradoresAux));
    Sinais *sinal = NULL;
    unsigned int escolha, tamLinhas, program_counter = 0, cont = 0; //UNSIGNED IMPOSSIBILITA QUE PROGRAM_COUNTER CHEGUE A MENOR QUE 0
    int StateForBack = -1, i = 0, Etapa = 1, auxiliar, verifica = 0;
    type_instruc *instrucoesDecodificadas = malloc(sizeof(type_instruc*));
    char dat[300]; //Recebe o nome do arquivo.dat
    //int *regs; //registradores como um inteiro mesmo
    //regs = (int*)malloc(8 * sizeof(int));
    int regs[8];
    for(int i=0;i<8;i++){//zerando registradores, caso contrario dá números inconsistentes
        regs[i] = 0;
    }
    printf("\n___________________________________________ ==ATENÇÃO== ____________________________________________\n\n");
    printf("| + TODOS OS ARQUIVOS DE INSTRUÇÂO DEVEM ESTAR NA PASTA 'memoria' COM O SEGUINTE NOME E EXTENSÂO + |\n");
    printf("__________________________________________ =INSTRUCT.mem= __________________________________________");
    do{
        printf("\n\n");
        printf("                              _____________________________________\n");
        printf("                              |1  +        Carregar memória        +|\n");
        printf("                              |2  +        Imprimir memória        +|\n");
        printf("                              |3  +      Imprimir registradores    +|\n");
        printf("                              |4  +      Imprimir estatisticas     +|\n");
        printf("                              |5  +  Imprimir instrucoes Assembly  +|\n");
        printf("                              |6  +    Imprimir todo o simulador   +|\n");
        printf("                              |7  +          Salvar .asm           +|\n");
        printf("                              |8  +          Salvar .dat           +|\n");
        printf("                              |9  +      Executa Programa (run)    +|\n");
        printf("                              |10 +    Executa instrucao (step)    +|\n");
        printf("                              |11 +   Volta uma instrucao (back)   +|\n");
        printf("                              |0  +              Sair              +|\n");
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 0:
            free(memorias);
            free(aux);
            for(i=0;i<8;i++){
                regs[i] = 0;
            }
            free(AssemblyInst);
            free(instrucoesDecodificadas);
            system("clear");
            printf("Programa Encerrado!\n");
            break;
            
        case 1: //Carregar memória de Instruções
            if (verifica == 0){
                memorias = inicializaMem();
                parser(memorias, &tamLinhas);

                AssemblyInst = calloc((tamLinhas + 1), sizeof(Assembly));
                if (AssemblyInst == NULL) {
                    fprintf(stderr, "Falha ao alocar memória para instrucoes assembly.\n");
                    return -1;
                }
                sinal = inicializaSinais();
                verifica++;
                }
            break;

        case 2: //Imprimir memórias
            imprimeMemoria(memorias);
            break;

        case 3: //Imprimir registradores
            imprimeRegistradores(regs);
            break;

        case 4: //Imprimir estatísticas como: quantas intruc, classes, etc;
            imprimeEstatisticas(memorias, tamLinhas, instrucoesDecodificadas, program_counter);
            printf("Etapa atual: %d\n", Etapa);
            break;
            
        case 5: // Imprimir Assembly
            imprimirASM(AssemblyInst, tamLinhas);
            break;

        case 6: //imprimir todo o simulador
            imprimeEstatisticas(memorias, tamLinhas, instrucoesDecodificadas, program_counter);
            imprimeSimulador(tamLinhas, instrucoesDecodificadas, memorias);      
            imprimeMemoria(memorias);
            imprimirASM(AssemblyInst, tamLinhas);
            imprimeRegistradores(regs);
            break;

        case 7: //salvar arquivo .asm (com as instruções traduzidas para a linguagem intermediária Assembly)
            if(memorias == NULL){
                printf("Instrucoes nao carregadas");
                break;
            }
            SaveASM(AssemblyInst, tamLinhas);
            break;

        case 8: //Salvar arquivo DATA.dat
            escreverArquivoMemoria(memorias);
            break;

        case 9: //Chamar função responsável pela execução do programa
            if(memorias == NULL){
                printf("Instrucoes nao carregadas");
                break;
            }
            Etapa = controller(1, &StateForBack, tamLinhas, regs, memorias, &program_counter, instrucoesDecodificadas, aux, &sinal, Etapa);
            AsmCopy(instrucoesDecodificadas, &AssemblyInst, tamLinhas);
            break;

        case 10: //Chamar função responsável pela execução do programa passo a passo
            if(memorias == NULL){
                printf("Instrucoes nao carregadas");
                break;
            }
            printf("Auxiliar pc: %d\n", aux->PC);
            printf("instrucao: %s\n", memorias[aux->PC].mem);
            printf("uso: %d\n", memorias[aux->PC].uso);
            Etapa = controller(2, &StateForBack, tamLinhas, regs, memorias, &program_counter, instrucoesDecodificadas, aux, &sinal, Etapa);
            AsmCopy(instrucoesDecodificadas, &AssemblyInst, tamLinhas);
            printf("\n"); 
            puts(AssemblyInst[aux->PC].InstructsAssembly);
            break;

        case 11: //Chamar função responsável por retornar uma instrução (PC--)
            printf("State: %d\n", StateForBack);
            if(memorias == NULL){
                printf("Instrucoes nao carregadas");
                break;
            }
            if (StateForBack <= 0){
                fprintf(stderr, "Usuario ja esta no inicio do programa.");
                StateForBack = -1;
                break;
            }
            memset(memorias, 0, sizeof(memorias)); //anula todo conteudo de md

            for (i = 0; i<8; i++){
                regs[i]=0;
            }
            Etapa = 1;

            auxiliar = StateForBack--;

            Etapa = controller(3, &StateForBack, auxiliar, regs, memorias, &program_counter, instrucoesDecodificadas, aux, &sinal, Etapa);

            puts(AssemblyInst[aux->PC].InstructsAssembly);
            break;
            
        default:
            break;
        }
    }while(escolha != 0);

    return 0;
}