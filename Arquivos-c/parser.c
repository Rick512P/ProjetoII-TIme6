#include "../Arquivos-h/parser.h"

//RESPONSAVEL POR ABRIR O ARQUIVO E ENCHER A MEMORIA DE INSTRUÇÕES
int parser(instrucao **memoriaInst, int *tamanho_linhas){
    char linha[100], nome_arquivo[200];
    int contador_de_linhas = 0, opcao;
    instrucao p;
    FILE *arq;
    printf("Digite 1 para utilizar o diretorio padrao ou 2 para entrar com o diretorio do arquivo: ");
    scanf("%d", &opcao);
    if(opcao == 1)
        arq = fopen("./memoria/testaTodas.mem", "r");
    else if(opcao == 2){
        printf("Entre com o caminho/nome do arquivo incluindo a extenção .mem\n");
        scanf(" %[^\n]", nome_arquivo);
        arq = fopen(nome_arquivo, "r");
    }
    else{
        printf("Opcao inexistente. Utilizando o diretorio padrao.");
        arq = fopen("./memoria/instrucoes.mem", "r");
    }

    if(arq){

        //primeiro, conto quantas linhas de instruçoes terei no arquivo
        while(fgets(linha, sizeof(linha), arq) != NULL){
            if (strlen(linha) > 17){
                fprintf(stderr, "OVERFLOW. Numero de bits maior que 16."); //FLAG OVERFLOW
                return -1;
            }
            contador_de_linhas++;
        }

        

        *tamanho_linhas = contador_de_linhas;

        //alocaçao de memoria que minha variavel memoriaInst terá será = ao tanto de linhas do arquivo lido
        *memoriaInst = malloc(256 * sizeof(instrucao));

        if (!memoriaInst) { //se memoriaInst nao ter nada alocado, dará erro.
            fprintf(stderr, "Falha na alocação de memória para instruções.\n"); //stderr envia mensagem de erro
            //separadamente do fluxo principal de saída de um programa
            return 1;
        }

        // Reinicia o arquivo para ler desde a 1° linha
        rewind(arq);


        for (int i=0;i<contador_de_linhas;i++){
            if (fgets(linha, sizeof(linha), arq) == NULL)
                break;
            

            linha[strcspn(linha, "\r\n")] = '\0';

            // Copia a linha para a estrutura memoriaInst
            strncpy((*memoriaInst)[i].instruc, linha, 17);
            (*memoriaInst)[i].instruc[sizeof((*memoriaInst)[i].instruc) - 1] = '\0'; // certifica-se de que a string termina com null terminator
        }
        if(contador_de_linhas <=256){
            for(int j=contador_de_linhas;j < 256; j++){
                strncpy((*memoriaInst)[j].instruc, "0000000000000000", 17);
            }

        }
        
        fclose(arq);
        printf("Arquivo lido com sucesso!");
    }
    

    else
        fprintf(stderr, "Erro ao abrir arquivo de instrucoes.");
}