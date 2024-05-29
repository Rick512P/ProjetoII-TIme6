#include "../Arquivos-h/decimal_to_bin.h"

void Complemento2(char *bin) {
    // Inverte os bits
    for (int i = 0; i < 8; i++) {
        bin[i] = (bin[i] == '0') ? '1' : '0';
    }

    // Adiciona um ao número binário
    int carry = 1;
    for (int i = 7; i >= 0; i--) {
        if (bin[i] == '1' && carry == 1) {
            bin[i] = '0';
        } else if (bin[i] == '0' && carry == 1) {
            bin[i] = '1';
            carry = 0; // Encerra após adicionar 1
            break;
        }
    }
}

void decimalToBinary(int num, char *bin) {
    int index = 7;
    int originalNum = num; // Guarda o número original para checar se é negativo
    num = (num >= 0) ? num : -num; // Usa o valor absoluto para a conversão inicial

    // Preenche os bits
    for (int i = 0; i < 8; i++) {
        bin[index--] = (num % 2) + '0';
        num /= 2;
    }
    bin[8] = '\0'; // Terminador nulo para a string

    // Aplica complemento de dois se o número original era negativo
    if (originalNum < 0) {
        Complemento2(bin);
    }
}
