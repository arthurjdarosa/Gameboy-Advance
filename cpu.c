#include "cpu.h"
#include <string.h>

void cpu_init(CPU *cpu, MMU *mmu){
    memset(cpu, 0, sizeof(CPU));
    cpu->mmu = mmu;
    cpu_reset(cpu);
}

void cpu_reset(CPU *cpu) {
    // AO ligar o 'console' sem pular a BIOS
    // O pc (R15) começa apontando para o endereço 0x00000000 (primeira instrução da BIOS)
    cpu->r[15] = 0x00000000;

    // O processador inicia no modo SUPERVISOR (0x13) em conjunto de instruções ARM (T = 0)
    // Desabilita interrupções IRQ e FIQ por padrão
    cpu->cpsr = MODE_SUPERVISOR | FLAG_I | FLAG_F;
}

bool cpu_get_flag(CPU *cpu, uint32_t flag){
    return (cpu->cpsr & flag) != 0;
}

void cpu_set_flag(CPU *cpu, uint32_t flag, bool value) {
    if (value) {
        cpu->cpsr |= flag; // Liga o bit correspondente
    } else {
        cpu->cpsr &= ~flag; // Desliga o bit correspondente
    }   
}

int cpu_step(CPU *cpu) {
    //1. Verifica o estado atual do processador (Modo ARM ou THUMB)
    if (cpu_get_flag(cpu, FLAG_T)) {
        // --- MODO THUMB (16 bits) ---
        uint16_t instr = mmu_read16(cpu->mmu, cpu->r[15]);

        // No modo THUMB, o pc avança 2 bytes por instrução
        cpu -> r[15] += 2;

        // Por enquanto, apenas silenciamos o warning de variavel não usada
        (void)instr;
        return 1; // 1 ciclo padrao
    } else {
        // --- MODO ARM (32 bits) ---
        // Busca a instrução de 32 bits no endereço aponstando pelo PC (R15)
        uint32_t instr = mmu_read32(cpu->mmu, cpu->r[15]);

        // No modo ARM, o PC avança 4 bytes por instrução
        cpu->r[15] += 4;

        // Por enquanto, apenas silenciamos o warning de variável não usada
        (void)instr;
        return 1; // 1 ciclo padrão
    }
}