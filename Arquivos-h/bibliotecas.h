#ifndef BIBLIOTECAS_H
#define BIBLIOTECAS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

    char inst[17];

}type_instruc;


//Memoria de dados e instrucoes unidas
typedef struct {
    char dados[17];
    char instruc[17];
} Memorias;


//instruções em assembly
typedef struct{
  char InstructsAssembly[30];
} Assembly;


typedef struct {
  char registradorInst[17];
  char registradorDados[17];
  char registradorA[4];
  char registradorB[7];
  int registradorResult;
} RegistradoresAux;

#endif
