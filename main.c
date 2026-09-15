#include <stdio.h>
#include <assert.h>
#include "mmu.h"
#include "cpu.h"

int main(void) {
    MMU mmu;
    CPU cpu;

    // 1. Inicializa os subsistemas
    mmu_init(&mmu);
    cpu_init(&cpu, &mmu);
    
    printf("=== Testando a MMU do GBA + CPU ===\n\n");

    // Teste 1: Escrita e leitura simples na IWRAM
    // mmu_write8(&mmu, 0x03000010, 0xAB);
    // uint8_t val8 = mmu_read8(&mmu, 0x03000010);
    // printf("1. Leitura 8-bit IWRAM: 0x%02X (Esperado: 0xAB)\n", val8);
    // assert(val8 == 0xAB);

    // 2. Teste do Programa (R15) no reset
    printf("1. R15 (PC): 0x%08X (Esperando: 0x00000000)\n", cpu.r[15]);
    assert(cpu.r[15] == 0x00000000);

    // Teste 2: Escrita de 32 bits e ordem Little_Endian
    // mmu_write32(&mmu, 0x03000100, 0x12345678);
    // uint32_t val32 = mmu_read32(&mmu, 0x03000100);
    // printf("2. Leitura 32-bit IWRAM: 0x%08X (Esperado: 0x12345678)\n", val32);
    // assert(val32 == 0x12345678);

    // 3. Teste do Mode Inicial no CPSR 0x13
    uint8_t modo_atual = (uint8_t)(cpu.cpsr & 0x1F);
    printf("2. Modo CPSR: 0x%02X (Esperado: 0x13 - Supervisor)\n", modo_atual);
    assert(modo_atual == MODE_SUPERVISOR);

    // Teste 3: Espelhamentos da EWRAM (256 KB)
    // Escrever em 0x02000000 e ler 256 KB depois (0x02040000) deve dar o mesmo valor
    // mmu_write8(&mmu, 0x02000000, 0x42);
    // uint8_t espelho = mmu_read8(&mmu, 0x02040000);
    // printf("3. Leitura 32-bit IWRAM: 0x%02X (Esperado: 0x42)\n", espelho);
    // assert(espelho == 0x42);

    // 4. Teste do estado das interrupções (IRQ e FIQ desabilitadas no boot)
    bool irq_desabilitada = cpu_get_flag(&cpu, FLAG_I);
    bool fiq_desabilitada = cpu_get_flag(&cpu, FLAG_F);
    printf("3. IRQ desabilitada: %s | FIQ desabilitada: %s\n",
            irq_desabilitada ? "SIM" : "NAO",
            fiq_desabilitada ? "SIM" : "NAO");
    assert(irq_desabilitada && fiq_desabilitada);

    // Teste 4: BIOS e Read-Only (escrita tem que ser ignorada)
    // mmu_write32(&mmu, 0x00000004, 0xFF);
    // uint32_t bios_val = mmu_read8(&mmu, 0x00000004);
    // printf("4. Protecao BIOS (Read-Only): 0x%08X (Esperado: 0xFF)\n", bios_val);
    // assert(bios_val == 0x00);

    // 5. Teste de manipulacao de flags (Zero e Carry)
    assert(!cpu_get_flag(&cpu, FLAG_Z));
    cpu_set_flag(&cpu, FLAG_Z, true);
    printf("4. FLAG_Z ligada : %s\n", cpu_get_flag(&cpu, FLAG_Z) ? "SIM" : "NAO");
    assert(cpu_get_flag(&cpu, FLAG_Z));

    // Desliga novamente para garantir que cpu_set_flag funciona nos dois sentidos
    cpu_set_flag(&cpu, FLAG_Z, false);
    assert(!cpu_get_flag(&cpu, FLAG_Z));

    // 6. Teste da ligacao CPU _> MMU
    // A CPU usa o ponteiro dela para gravar na memoria
    mmu_write32(cpu.mmu, 0x03000020, 0xCAFEBABE);
    uint32_t val = mmu_read32(cpu.mmu, 0x03000020);
    printf("5. Leitura via ponteiro cpu.mmu: 0x%08X (Esperado: 0xCAFEBABE)\n", val);
    assert(val == 0xCAFEBABE);

    mmu_free(&mmu);
    printf("\nTodos os testes passaram! CPU + MMU\n");
    return 0;
}

