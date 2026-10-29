# MMU.C
se o mmu.h foi o contrato com regras e tal esse é onde ele gerencia o que acontece quando a CPU tenta ler ou escrever em um endereço da memoria

void mmu_init(MMU *mmu) {
    *memset(mmu,0,sizeof(MMU));*
    limpa  a BIOS, EWRAM, VRAM... de qualquer lixo eletronico
    *mmu->rom = NULL;* 
    começa apontando para o vazio quando inicia o jogo
    *mmu->rom_zize = 0;*
    garante que o ponteiro da ROM comece vazio e o tamanho seja zero
}

void mmu_free(MMU *mmu) {
    if (mmu->rom){
        *free(mmu->rom);*
        *mmu->rom = NULL;*
    }
    Se a ROM foi carregada (não é NULL), devolve a memoria RAM ao sistema
}

*memset(mmu,0,sizeof(MMU))* = struct MMU guarda todos os arrays de memoria (BIOS, EWRAM, VRAM, etc.) O memset pega esse bloco gigante na memoria RAM do PC e preenche cada byte dele com o numero 0.

*uint8_t region* = (uint8_t)(addr >> 24);
O operador >> 24 empurra o numero 24 casas para direita sobrando apenas os doois primeiros caracteres hexadeciamis do endereço (por exemplo, se o endereço for 0x03000004, o addr >> 24 resulta exatamente em 0x03)

*case 0x00 (BIOS)* = verifica o endereço esta dentro do limite de 16 KB (if (addr < BIOS_SIZE)). se estiver devolve o byte correspondente guardado no array mmu->bios[addr].

*case 0x02 e 0x03 (EWRAM e IWRAM)* = Como a RAM repete seus dados se o endereço passar do limite fisico essa conta força o numero a dar a volta e cair dentro do tamanho real do array.

*case 0x05, 0x06, 0x07 (PRAM, VRAM e OAM)* = As memórias de vídeo e paletas de cores do console, também protegidas por máscaras de tamanho e espelhamento para evitar que o programa acesse posições inexistentes.

*case 0x08 até 0x0D (Game Pak ROM / Cartucho)* = Aqui o GBA lê os dados do cartucho do jogo. Ele isola os 25 bits inferiores (addr & 0x01FFFFFF) para suportar cartuchos de até 32 MB, valida se o jogo realmente foi carregado (mmu->rom) e se o endereço pedido não ultrapassa o tamanho do arquivo do jogo (offset < mmu->rom_size).

*default* = Se a CPU tentar ler um endereço que não existe em nenhum chip da placa o emulador retorna 0 por segurança (comportamento de open-bus simplificado)

O processador ARM lê o mundo não apenas de byte em byte (8 bits), mas também em blocos de 16 bits (Halfword) e 32 bits (Word). É aqui que entra o alinhamento e a ordem Little-Endian:

*uint16_t mmu_read16(MMU *mmu, uint32_t addr) {addr &= ~1; // Força o alinhamento para múltiplo de 2 return (uint16_t)(mmu_read8(mmu, addr) | (mmu_read8(mmu, addr + 1)<< 8));}* 

O que faz: Para ler 16 bits, a função lê dois bytes separados usando a função mmu_read8: o byte do endereço base (addr) e o byte seguinte (addr + 1).

O deslocamento << 8: Como o GBA é Little-Endian (o byte menos significativo vem primeiro na memória), pegamos o segundo byte, empurramos ele 8 posições para a esquerda (<< 8) e juntamos com o primeiro usando o operador de "OU" binário (|).