#include <stdio.h>
#include <assert.h>
#include "mmu.h"

int main(void) {
    MMU mmu;
    mmu_init(&mmu);
    

    printf("=== Testando a MMU do GBA ===\n\n");

    // Teste 1: Escrita e leitura simples na IWRAM
    mmu_write8(&mmu, 0x03000010, 0xAB);
    uint8_t val8 = mmu_read8(&mmu, 0x03000010);
    printf("1. Leitura 8-bit IWRAM: 0x%02X (Esperado: 0xAB)\n", val8);
    assert(val8 == 0xAB);

    // Teste 2: Escrita de 32 bits e ordem Little_Endian
    mmu_write32(&mmu, 0x03000100, 0x12345678);
    uint32_t val32 = mmu_read32(&mmu, 0x03000100);
    printf("2. Leitura 32-bit IWRAM: 0x%08X (Esperado: 0x12345678)\n", val32);
    assert(val32 == 0x12345678);

    // Teste 3: Espelhamentos da EWRAM (256 KB)
    // Escrever em 0x02000000 e ler 256 KB depois (0x02040000) deve dar o mesmo valor
    mmu_write8(&mmu, 0x02000000, 0x42);
    uint8_t espelho = mmu_read8(&mmu, 0x02040000);
    printf("3. Leitura 32-bit IWRAM: 0x%02X (Esperado: 0x42)\n", espelho);
    assert(espelho == 0x42);

    // Teste 4: BIOS e Read-Only (escrita tem que ser ignorada)
    mmu_write32(&mmu, 0x00000004, 0xFF);
    uint32_t bios_val = mmu_read8(&mmu, 0x00000004);
    printf("4. Protecao BIOS (Read-Only): 0x%08X (Esperado: 0xFF)\n", bios_val);
    assert(bios_val == 0x00);

    mmu_free(&mmu);
    printf("\nTodos os testes passaram! A memoria esta pronta.\n");
    return 0;
}

