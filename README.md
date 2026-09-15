# GAMEBOY
Projeto para aprender mais sobre linguagem C
Por que existe um arquivo .h (Header File)? 

O compilador lê o arquivo .c quando o GCC (Compiler Collection) compila o main.c, ele não olha o que esta escrito dentro do mmu.c .
O compilador precisa de garantias se o main.c chamar mmu_read32(...) o compilador precisa saber com antecedencia se essa função existe basicamente

O arquivo .h é o contrato o .h header ou cabeçalho contem apenas as funções do modulo, ele nao contem o codigo do que a função isso fica no .c

No .h eu digo "funcao chamada mmu_read32 que recebe um endereço e devolve um numero de 32bits"
no mmu.c voce escreve o corpo da funcao
o main.c inclui (#include "mmu.h") apenas esse contrato assim o compilador confia que a função existe e deixa compilar sem erros
