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