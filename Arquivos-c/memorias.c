#include "../Arquivos-h/memorias.h"


type_instruc Memoria(RegistradoresAux *aux){
        type_instruc traduzido; //DECOMPOE A INSTRUÇÃO EM OPCODE, RS, RT, RD, FUNCT, IMM OU ADDR

        traduzido = decoder(aux); //DECODER IRA DECOMPOR A INSTRUÇÃO

		
        return traduzido; //retorna para o controller

}

void imprimeMemInstruc(Memorias *memoriaInst, int tamLinhas){
    for (int i=0; i<tamLinhas;i++){
                printf("Instrucao %d: %s\n\n", i, memoriaInst[i].instruc);
            }
}

void escreveDado(Memorias **mem, int immediate, char *valor) { //ESCREVE O DADO DA POSICAO DE MEMORIA 128 -> 256
    if (immediate >= 0 && immediate < 256) {
        strcat((*mem)[immediate].dados, valor);
    } else {
        fprintf(stderr, "Tentativa de escrita fora dos limites da memória no endereço %d\n", immediate);
    }
}

void imprimeDados(Memorias *md, int tamLinhas){
	for (int i = 0; i < 256; i++) {
		printf("Dado armazenado no endereco %d da MD: [%s]\n", i, md[i].dados);
	}
}

void escreverArquivoMemoria(Memorias *md) {
	FILE *arquivo;
	arquivo = fopen("./DATA.dat", "w");
	
	if(arquivo==NULL){
		fprintf(stderr, "Erro ao abrir o arquivo DATA.dat\n");
	}
	
	for (int i=0; i<256; i++){
		fprintf(arquivo, "%s\n", md[i].dados);
	}
	
	fclose(arquivo);
	printf("\nArquivo criado com sucesso!\n\n");
}


    

