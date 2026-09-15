# MMU.H
#ifndef MMU_H
#define MMU_H

chamados de include guards
como funciona: #ifndef MMU_H pergunta: "O rotulo MMU_H já foi definido antes?" Se for a primeira vez #define MMU_H cria essa marcao se o compilador encontrar esse arquivo de novo na mesma compilação ele pula direto para o #endif no final

emulando um hardware com registradores e barramentos rígidos (bytes exatos de 8, 16 e 32 bits), importamos <stdint.h>, que nos dá tipos de tamanho exato garantido:
uint8_t: inteiro sem sinal (unsigned) de 8 bits (1 byte)
uint16_t: inteiro sem sinal de 16 bits (2 bytes)
uint32_t: inteiro sem sinal de 32 bits (4 bytes)




