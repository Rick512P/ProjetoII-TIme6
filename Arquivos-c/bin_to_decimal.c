#include "../Arquivos-h/bin_to_decimal.h"


#include <stdio.h>
#include <string.h>

int bin_to_decimal(char *binario) {
    int dec = 0;
    int base = 1;
    int len = strlen(binario);
    char bitsig;

    if(len > 3){
        if(binario[0] == '1'){
            ComplementodeDois(binario);
            for (int i = len - 1; i >= 0; i--) {
                if (binario[i] == '1') {
                    dec += base;
                }
                base *= 2;
            }
            dec = dec * -1;
        }

        else{
            for (int i = len - 1; i >= 0; i--) {
                if (binario[i] == '1') {
                    dec += base;
                }
            base *= 2;
            }
        }
    }

    else {
        for (int i = len - 1; i >= 0; i--) {
                if (binario[i] == '1') {
                    dec += base;
                }
            base *= 2;
            }
    }
    
    return dec;
}


// Função recursiva para inverter os bits de uma string
void invertBits(char *str, int index) {
    if (str[index] == '\0') {
        return; // Caso base: fim da string
    }
    // Inverte o bit atual
    str[index] = (str[index] == '0') ? '1' : '0';
    // Chamada recursiva para o próximo caractere
    invertBits(str, index + 1);
}

// Função recursiva para adicionar 1 a um número binário representado como string
void addOne(char *str, int index) {
    if (index < 0) {
        return; // Caso base: todos os bits foram processados
    }
    if (str[index] == '1') {
        str[index] = '0'; // Se for 1, vira 0 e o "carry" continua
        addOne(str, index - 1);
    } else {
        str[index] = '1'; // Se for 0, vira 1 e o "carry" é consumido
    }
}

// Função para converter complemento de dois para valor positivo
void ComplementodeDois(char *bin) {
    invertBits(bin, 0); // Inverte todos os bits
    addOne(bin, strlen(bin) - 1); // Adiciona 1 ao número invertido
}