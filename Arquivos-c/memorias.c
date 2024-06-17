#include "../Arquivos-h/memorias.h"


type_instruc Memoria(RegistradoresAux *aux){
        type_instruc traduzido; //DECOMPOE A INSTRUÇÃO EM OPCODE, RS, RT, RD, FUNCT, IMM OU ADDR

        traduzido = decoder(aux); //DECODER IRA DECOMPOR A INSTRUÇÃO

		
        return traduzido; //retorna para o controller

}

void escreveDado(Memorias **mem, int immediate, char *valor) { //ESCREVE O DADO DA POSICAO DE MEMORIA 128 -> 256
    if (immediate >= 0 && immediate < 256) {
		if((*mem)[immediate].uso != 'i'){ //se o uso nao for i, entao sera usado para dados ('d')
			strcpy((*mem)[immediate].dados, "00000000");
        	strcat((*mem)[immediate].dados, valor);
			(*mem)[immediate].uso = 'd';
		}
		else{
			int aux = 0, original;
			original = immediate;
			while((*mem)[immediate].uso == 'i'){
				fprintf(stderr, "Tentativa de escrita em endereço ja utilizado por uma instrucao.\n");
				immediate++;
				if ((*mem)[immediate].uso != 'i'){
					aux = 1;
				}
			}
			if(aux == 1){ //se aux == 1, achamos um local para escrever o dado na memoria
				strcat((*mem)[immediate].dados, "00000000");
        		strcat((*mem)[immediate].dados, valor);
				(*mem)[immediate].uso = 'd';
				printf("Escrita realizada no endereço %d ao inves de %d\n", immediate, original);
			}
			else {
				fprintf(stderr, "Local na memoria nao encontrado para escrita de dados\n");
			}
			
		}
		
    } else {
        fprintf(stderr, "Tentativa de escrita fora dos limites da memória no endereço %d\n", immediate);
    }
}

void imprimeMemoria(Memorias *md){
	for (int i = 0; i < 256; i++) {
		if(md[i].uso == 'd'){
			if (i < 10)
				printf("Dado armazenado no endereco 00%d da MD: [%s]\n", i, md[i].dados);
			else if (i<100)
				printf("Dado armazenado no endereco 0%d da MD: [%s]\n", i, md[i].dados);
			else
				printf("Dado armazenado no endereco 0%d da MD: [%s]\n", i, md[i].dados);
		}
		else{
			if (i < 10)
				printf("Dado armazenado no endereco 00%d da MD: [%s]\n", i, md[i].instruc);
			else if (i<100)
				printf("Dado armazenado no endereco 0%d da MD: [%s]\n", i, md[i].instruc);
			else
				printf("Dado armazenado no endereco %d da MD: [%s]\n", i, md[i].instruc);
			
		}
	}
}

void escreverArquivoMemoria(Memorias *md) {
	FILE *arquivo;
	arquivo = fopen("./DATA.dat", "w");
	
	if(arquivo==NULL){
		fprintf(stderr, "Erro ao abrir o arquivo DATA.dat\n");
	}
	
	for (int i=0; i<256; i++){
		if(md[i].uso == 'd')
			fprintf(arquivo, "%s\n", md[i].dados);
		else{
			fprintf(arquivo, "\n");
		}
	}
	
	fclose(arquivo);
	printf("\nArquivo criado com sucesso!\n\n");
}


    

