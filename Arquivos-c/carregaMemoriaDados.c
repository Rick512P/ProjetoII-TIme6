#include "../Arquivos-h/carregaMemoriaDados.h"


char* carregamd(MemoriaDados **md){
    FILE *setmd;
    char linha[1024];
    char* nome_arquivo = (char*)malloc(300 * sizeof(char));
    int contador_de_linhas = 0, opcao, error = 0;
    strcpy(nome_arquivo,"./memoria/md.dat");

    printf("Digite 1 para utilizar o diretorio padrao ou 2 para entrar com o diretorio do arquivo: ");
    scanf("%d", &opcao);
    if(opcao == 1)
        setmd = fopen(nome_arquivo, "r");
    else if(opcao == 2){
        printf("Entre com o caminho/nome do arquivo incluindo a extenção .dat\n");
        scanf(" %[^\n]", nome_arquivo);
        setmd = fopen(nome_arquivo, "r");
    }
    else{
        printf("Opcao inexistente. Utilizando o diretorio padrao.\n");
        setmd = fopen(nome_arquivo, "r");
    }


    if (setmd){
        
        //primeiro, conto quantas linhas de dados terei no arquivo
        while(fgets(linha, sizeof(linha), setmd) != NULL){
            if (strlen(linha) > 8){
                fprintf(stderr, "OVERFLOW. Linha %d tem mais de 8 caracteres: %s\n", contador_de_linhas, linha); //FLAG OVERFLOW
                error=1;
            }
                
            contador_de_linhas++;
        }

        
        if (md == NULL)
            *md = malloc(256 * sizeof(MemoriaDados));
        
        rewind(setmd);
        
        for (int i=0;i<contador_de_linhas;i++){
            if (fgets(linha, sizeof(linha), setmd) == NULL)
                   break;

            linha[strcspn(linha, "\r\n")] = '\0';

            // Copia a linha para a estrutura memoria de Dados
            strncpy((*md)[i].dados, linha, 9);
            (*md)[i].dados[sizeof((*md)[i].dados) - 1] = '\0'; // certifica-se de que a string termina com null terminator
            
        }
        fclose(setmd);
        printf("Arquivo lido com sucesso!\n");
        
    }
    else
        fprintf(stderr, "Erro ao abrir arquivo md.dat\n");
    if (error == 0)
        return nome_arquivo;
    else{
        strcpy(nome_arquivo, "ERRO");
        return nome_arquivo;
    }
}

void recarregarmd(MemoriaDados **md, char *nome_arquivo){
    FILE *setmd;
    char linha[1024];
    int contador_de_linhas = 0, opcao;

    setmd = fopen(nome_arquivo, "r");

    if (setmd){
        
        //primeiro, conto quantas linhas de dados terei no arquivo
        while(fgets(linha, sizeof(linha), setmd) != NULL){
            contador_de_linhas++;
        }

        
        if (md == NULL)
            *md = malloc(256 * sizeof(MemoriaDados));
        
        rewind(setmd);
        
        for (int i=0;i<contador_de_linhas;i++){
            if (fgets(linha, sizeof(linha), setmd) == NULL)
                   break;

            linha[strcspn(linha, "\r\n")] = '\0';

            // Copia a linha para a estrutura memoria de Dados
            strncpy((*md)[i].dados, linha, 9);
            (*md)[i].dados[sizeof((*md)[i].dados) - 1] = '\0'; // certifica-se de que a string termina com null terminator
            
        }
        fclose(setmd);        
    }

}