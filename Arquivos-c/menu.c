#include "../Arquivos-h/menu.h"

int main(){
    menu();
    return 0;
}

int menu(){
    Assembly *AssemblyInst;
    Memorias *memorias = NULL;
    RegistradoresAux *aux = malloc(sizeof(RegistradoresAux));
    Sinais *sinal = NULL;
    unsigned int escolha, tamLinhas, program_counter = 0, cont = 0; //UNSIGNED IMPOSSIBILITA QUE PROGRAM_COUNTER CHEGUE A MENOR QUE 0
    int StateForBack = -1, i = 0, Etapa = 1, auxiliar, verifica = 0;
    type_instruc **instrucoesDecodificadas = malloc(sizeof(type_instruc*));
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
        printf("                              |1  +  Carregar memória instruções   +|\n");
        printf("                              |2  +    Carregar memoria dados      +|\n");
        printf("                              |3  +        Imprimir memória        +|\n");
        printf("                              |4  +      Imprimir registradores    +|\n");
        printf("                              |5  +      Imprimir estatisticas     +|\n");
        printf("                              |6  +  Imprimir instrucoes Assembly  +|\n");
        printf("                              |7  +    Imprimir todo o simulador   +|\n");
        printf("                              |8  +          Salvar .asm           +|\n");
        printf("                              |9  +          Salvar .dat           +|\n");
        printf("                              |10 +     Executa Programa (run)     +|\n");
        printf("                              |11 +    Executa instrucao (step)    +|\n");
        printf("                              |12 +   Volta uma instrucao (back)   +|\n");
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
                *instrucoesDecodificadas = malloc(tamLinhas * sizeof(type_instruc));
                if (*instrucoesDecodificadas == NULL) {
                    fprintf(stderr, "Falha ao alocar memória para instruções decodificadas.\n");
                    return -1;
                }

                AssemblyInst = calloc((tamLinhas + 1), sizeof(Assembly));
                if (AssemblyInst == NULL) {
                    fprintf(stderr, "Falha ao alocar memória para instrucoes assembly.\n");
                    return -1;
                }
                sinal = inicializaSinais();
                verifica++;
                }
            break;

        case 2: //Carregar Memória de Dados
            if(memorias == NULL){
                memorias = inicializaMem();
            }
            if (program_counter == 0){
                strcpy(dat,carregaDados(memorias));
                printf("\n");
                puts(dat);
                printf("\n");
                if (strcmp(dat,"ERRO")!=0){
                    cont = 1;
                }
            }
            else 
                printf("Programa nao deve ja ter sido inicializado.");
            break;

        case 3: //Imprimir memória de instruções e memória de dados
            imprimeMemoria(memorias);
            break;

        case 4: //Imprimir registradores
            imprimeRegistradores(regs);
            break;

        case 5: //Imprimir estatísticas como: quantas intruc, classes, etc;
            imprimeEstatisticas(memorias, tamLinhas, instrucoesDecodificadas, program_counter);
            printf("Etapa atual: %d\n", Etapa);
            break;
            
        case 6: // Imprimir Assembly
            imprimirASM(AssemblyInst, tamLinhas);
            break;

        case 7: //imprimir todo o simulador
            imprimeEstatisticas(memorias, tamLinhas, instrucoesDecodificadas, program_counter);
            imprimeSimulador(tamLinhas, instrucoesDecodificadas, memorias);      
            imprimeMemoria(memorias);
            imprimirASM(AssemblyInst, tamLinhas);
            imprimeRegistradores(regs);
            break;

        case 8: //salvar arquivo .asm (com as instruções traduzidas para a linguagem intermediária Assembly)
            if(memorias == NULL){
                printf("Instrucoes nao carregadas");
                break;
            }
            SaveASM(AssemblyInst, tamLinhas);
            break;

        case 9: //Salvar arquivo DATA.dat
            escreverArquivoMemoria(memorias);
            break;

        case 10: //Chamar função responsável pela execução do programa
            if(memorias == NULL){
                printf("Instrucoes nao carregadas");
                break;
            }
            Etapa = controller(1, &StateForBack, tamLinhas, regs, memorias, &program_counter, instrucoesDecodificadas, &aux, &sinal, Etapa);
            AsmCopy(instrucoesDecodificadas, &AssemblyInst, tamLinhas);
            break;

        case 11: //Chamar função responsável pela execução do programa passo a passo
            if(memorias == NULL){
                printf("Instrucoes nao carregadas");
                break;
            }
            printf("instrucao: %s\n", memorias->mem);
            printf("uso: %d\n", memorias->uso);
            Etapa = controller(2, &StateForBack, tamLinhas, regs, memorias, &program_counter, instrucoesDecodificadas, &aux, &sinal, Etapa);
            AsmCopy(instrucoesDecodificadas, &AssemblyInst, tamLinhas);
            printf("\n"); 
            puts(AssemblyInst[aux->PC].InstructsAssembly);
            break;

        case 12: //Chamar função responsável por retornar uma instrução (PC--)
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

            Etapa = controller(3, &StateForBack, auxiliar, regs, memorias, &program_counter, instrucoesDecodificadas, &aux, &sinal, Etapa);

            puts(AssemblyInst[aux->PC].InstructsAssembly);
            break;
            
        default:
            break;
        }
    }while(escolha != 0);

    return 0;
}