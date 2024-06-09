#include "../Arquivos-h/carregaMemoriaDados.h"

char* carregamemoria(Memorias **memoria){
    FILE *setmemoria;
    char linha[1024];
    char* nome_arquivo = (char*)malloc(300 * sizeof(char));
    int contador_de_linhas = 0, opcao, error = 0;
    strcpy(nome_arquivo,"./memoria/memoria.dat");

    printf("Digite 1 para utilizar o diretorio padrao ou 2 para entrar com o diretorio do arquivo: ");
    scanf("%d", &opcao);
    if(opcao == 1)
        setmemoria = fopen(nome_arquivo, "r");
    else if(opcao == 2){
        printf("Entre com o caminho/nome do arquivo incluindo a extenção .dat\n");
        scanf(" %[^\n]", nome_arquivo);
        setmemoria = fopen(nome_arquivo, "r");
    }
    else{
        printf("Opcao inexistente. Utilizando o diretorio padrao.\n");
        setmemoria = fopen(nome_arquivo, "r");
    }


    if (setmemoria){
        
        //primeiro, conto quantas linhas de dados terei no arquivo
        while(fgets(linha, sizeof(linha), setmemoria) != NULL){
            if (strlen(linha) > 16){
                fprintf(stderr, "OVERFLOW. Linha %d tem mais de 16 caracteres: %s\n", contador_de_linhas, linha); //FLAG OVERFLOW
                error=1;
            }
                
            contador_de_linhas++;
        }

        
        if (*memoria == NULL)
            *memoria = malloc(contador_de_linhas * sizeof(Memorias));
        
        rewind(setmemoria);
        int i = 0;
        while(i<contador_de_linhas && i <= 255){
            if (fgets(linha, sizeof(linha), setmemoria) == NULL)
                break;

            linha[strcspn(linha, "\r\n")] = '\0';

            // Copia a linha para a estrutura memoria de Dados
            strncpy((*memoria)[i].dados, linha, 17);
            (*memoria)[i].dados[sizeof((*memoria)[i].dados) - 1] = '\0'; // certifica-se de que a string termina com null terminator
            i++;
            if(i == 255)
                printf("\nNumero de instrucoes atingiu limite maximo na memoria\n");
        }
        fclose(setmemoria);
        printf("Arquivo lido com sucesso!\n");
        
    }
    else
        fprintf(stderr, "Erro ao abrir arquivo memoria.dat\n");
    if (error == 0)
        return nome_arquivo;
    else{
        strcpy(nome_arquivo, "ERRO");
        return nome_arquivo;
    }
}

void escreverArquivoMemoria(Memorias *memoria) {
    FILE *arquivo;
    arquivo = fopen("./DATA.dat", "w");
    
    if(arquivo==NULL){
        fprintf(stderr, "Erro ao abrir o arquivo DATA.dat\n");
        return;
    }
    
    for (int i=0; i<256; i++){
        fprintf(arquivo, "%s\n", memoria[i].dados);
    }
    
    fclose(arquivo);
    printf("\nArquivo criado com sucesso!\n\n");
}
