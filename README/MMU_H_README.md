# MMU.H
#ifndef MMU_H
#define MMU_H

chamados de include guards
como funciona: #ifndef MMU_H pergunta: "O rotulo MMU_H já foi definido antes?" Se for a primeira vez #define MMU_H cria essa marcao se o compilador encontrar esse arquivo de novo na mesma compilação ele pula direto para o #endif no final

emulando um hardware com registradores e barramentos rígidos (bytes exatos de 8, 16 e 32 bits), importamos <stdint.h>, que nos dá tipos de tamanho exato garantido:
uint8_t: inteiro sem sinal (unsigned) de 8 bits (1 byte)
uint16_t: inteiro sem sinal de 16 bits (2 bytes)
uint32_t: inteiro sem sinal de 32 bits (4 bytes)

<stddef.h> traz a definição de size_t (usado para tamanho de memoria que usamos no #define item_x (a * b))

o uint_8 foi introduzido por conta do stdint.h ele garante que a variavel vai ter sempre 1byte(8bits) idependente se voce rodar ele em um computador de 32bits, 64bits ou sla uma batata

#define é uma instrução do pré-processador para criar constantes literais. Antes de compilar o compilador substitui a palavra pelo numero é uma variavel tipo var = 67 (20 + 20 + 20 + 7) so que no C

Os numeros representam as regioes da memroai reais da placa do gameboy mas ja ta la no codigo mesmo as quantidades

struct: uma estrutura em C é o mais proximo de uma classe simples ou dicionario fixo, ela agrupa varias variaveis sob o mesmo teto na memoria 

e o que tem dentro da struct uint_8_t bios[BIOS_SIZE] reserva um bloco continuo de bytes crus na memoriapara simular o chip de BIOS, e isso vale para todos ali, eu meio que defino variaveis estaticas e posso usar ela, antes tinha definido os tamanhos agora deixo elas com esse tamanho estatico

o * indica um ponteiro entao eu aloco rom de antemao deixando o ponteiro apontando para o vazio (null) e, quando escolher o arquivo .gba o malloc reserva exatamente a quantidade de memroia necessaria ja que ele pode ter 4, 8, 16 ou 32 MB

o typedef ... MMU permite que eu use apenas o nome mmu ao declaras as variaveis em vez de ter que digitar struct mmu toda vez 

mmu_init recebe o ponteiro da struct MMU *mmu para zerar todas as memorias e liga

mmu_free se tiver carregado um arquivo de jogo na memoria (rom) essa função devolve essa memoria ao sistema operacional para nao vazer memroia RAM do computador

#endif fecha o bloco aberto lá na primeira linha #ifndef MMU_H



