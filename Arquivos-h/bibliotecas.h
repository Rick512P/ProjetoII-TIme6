#ifndef BIBLIOTECAS_H
#define BIBLIOTECAS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//instrucao da memoria de instruções
typedef struct{
    char instruc[17];

}instrucao;


//intrucao traduzida
typedef struct{
    char opcode[5];

    //TIPO R OU I
    char rs [4];
    char rt [4];

    //TIPO R
    char rd [4];
    char funct [4];

    //TIPO I
    char imm [7];

    //TIPO J
    char addr[8];

}type_instruc;


//tamanho da memoria de dados
typedef struct {
    char dados[9];
} MemoriaDados;

//instruções em assembly
typedef struct{
  char InstructsAssembly[30];
} Assembly;



#endif
