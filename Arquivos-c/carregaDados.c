#include "../Arquivos-h/carregaDados.h"

char* carregaDados(Memorias **memoria){
    FILE *setmemoria;
    char linha[1024];
    char* nome_arquivo = (char*)malloc(300 * sizeof(char));
    int contador_de_linhas = 0, opcao, error = 0;
    strcpy(nome_arquivo,"./memoria/md.dat");

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
            if (strlen(linha) > 17){
                fprintf(stderr, "OVERFLOW. Linha %d tem mais de 16 caracteres: %s\n", contador_de_linhas, linha); //FLAG OVERFLOW
                error=1;
            }
                
            contador_de_linhas++;
        }
        
        rewind(setmemoria);
        int i = 0;
        while(i<contador_de_linhas && i <= 255){
            if (fgets(linha, sizeof(linha), setmemoria) == NULL){
                remove_newline(linha);
                break;
            }
            
            if(linha[0] == '\n'){
                i++;
            }

            else if((*memoria)[i].uso == '\0'){ //se o uso nao for i, entao sera usado para dados ('d')
                strncpy((*memoria)[i].dados, linha, 17);
                (*memoria)[i].dados[sizeof((*memoria)[i].dados) - 1] = '\0'; // certifica-se de que a string termina com null terminator
                (*memoria)[i].uso = 'd';
            }
            else{
                int posicao = -1, original;
                original = i;
                while((*memoria)[i].uso != '\0'){
                    fprintf(stderr, "Tentativa de escrita em endereço ja utilizado por uma instrucao/dado.\n");
                    i++;
                    if ((*memoria)[i].uso == '\0'){
                        posicao = i;
                        break;
                    }
                }
                if(posicao == -1){
                    fprintf(stderr, "Espaco nao encontrado na memoria.\n");
                    return NULL;
                }
                strncpy((*memoria)[i].dados, linha, 17);
                (*memoria)[i].dados[sizeof((*memoria)[i].dados) - 1] = '\0'; // certifica-se de que a string termina com null terminator
                (*memoria)[i].uso = 'd';
                printf("Escrita realizada no endereço %d ao inves de %d\n", i, original);
                i = original;
            }
            
            linha[strcspn(linha, "\r\n")] = '\0';

            // Copia a linha para a estrutura memoria de Dados
            
            i++;
            if(i == 255){
                    printf("\nNumero de instrucoes atingiu limite maximo na memoria\n");
                    break;
            }
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


void recarregarmd(Memorias **md, char *nome_arquivo){
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
            *md = malloc(256 * sizeof(Memorias));
        
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


void remove_newline(char *line) { //remove a quebra de linha
    size_t len = strcspn(line, "\n");
    if (line[len] == '\n') {
        line[len] = '\0';
    }
}