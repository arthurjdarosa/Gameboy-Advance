# MMU.C
se o mmu.h foi o contrato com regras e tal esse é onde ele gerencia o que acontece quando a CPU tenta ler ou escrever em um endereço da memoria

void mmu_init(MMU *mmu) {
    *memset(mmu,0,sizeof(MMU));*
    limpa  a BIOS, EWRAM, VRAM... de qualquer lixo eletronico
    *mmu->rom = NULL;*
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



