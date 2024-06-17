#include "../Arquivos-h/memorias.h"


type_instruc Memoria(RegistradoresAux *aux){
        type_instruc traduzido; //DECOMPOE A INSTRUÇÃO EM OPCODE, RS, RT, RD, FUNCT, IMM OU ADDR

        traduzido = decoder(aux); //DECODER IRA DECOMPOR A INSTRUÇÃO

        return traduzido; //retorna para o controller
}

void escreveDado(Memorias **mem, int immediate, char *valor) { //ESCREVE O DADO DA POSICAO DE MEMORIA 128 -> 256
    if (immediate >= 0 && immediate < 256) {
		if((*mem)[immediate].uso == '\0'){ //se o uso foi \0, entao sera usado para dados ('d')
			strcpy((*mem)[immediate].mem, "00000000");
        	strcat((*mem)[immediate].mem, valor);
			(*mem)[immediate].uso = 'd';
		}
		else{
			fprintf(stderr, "Tentativa de escrita em endereço ja utilizado por uma instrucao/dado.\n");
		}	
	}
	else {
        fprintf(stderr, "Tentativa de escrita fora dos limites da memória no endereço %d\n", immediate);
    }
}

void imprimeMemoria(Memorias *md){
	for (int i = 0; i < 256; i++) {
		if(md[i].uso == 'd'){
			if (i < 10)
				printf("Dado armazenado no endereco 00%d da MD: [%s]\n", i, md[i].mem);
			else if (i<100)
				printf("Dado armazenado no endereco 0%d da MD: [%s]\n", i, md[i].mem);
			else
				printf("Dado armazenado no endereco %d da MD: [%s]\n", i, md[i].mem);
		}
		else{
			if (i < 10)
				printf("Instrucao armazenada no endereco 00%d da MD: [%s]\n", i, md[i].mem);
			else if (i<100)
				printf("Instrucao armazenada no endereco 0%d da MD: [%s]\n", i, md[i].mem);
			else
				printf("Instrucao armazenada no endereco %d da MD: [%s]\n", i, md[i].mem);
			
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
			fprintf(arquivo, "%s\n", md[i].mem);
		else{
			fprintf(arquivo, "\n");
		}
	}
	
	fclose(arquivo);
	printf("\nArquivo criado com sucesso!\n\n");
}


    

