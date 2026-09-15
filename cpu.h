#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>
#include "mmu.h"

// Modos de operação do processar ARM
typedef enum {
    MODE_USER = 0X10,
    MODE_FIQ = 0X11,
    MODE_IRQ = 0X12,
    MODE_SUPERVISOR = 0X13,
    MODE_ABORT = 0X17,
    MODE_UNDEFINED = 0X1B,
    MODE_SYSTEM = 0X1F
}CpuMode;

// Mascaras de bits para as flags do CPSR? 
#define FLAG_N (1U << 31) //Negative
#define FLAG_Z (1U << 30) //Zero
#define FLAG_C (1U << 29) //Carry
#define FLAG_V (1U << 28) //Overflow
#define FLAG_I (1U << 7) //IRQ Disable
#define FLAG_F (1U << 6) //FIQ Disable
#define FLAG_T (1U << 5) //stade (0: ARM 32-bit, 1: thumb 16-bit)

typedef struct {
    // Registradoes acessiveis no modo atual (R0 - R15)
    uint32_t r[16];

    // Registrador  de status atual
    uint32_t cpsr;

    // Registrador de status salvo (usado ao entrar em interrupções)
    uint32_t spsr;

    // Registradores sombreados (baked) para troca de modo de operacao
    uint32_t r13_bank[6];
    uint32_t r14_bank[6];
    uint32_t spsr_bank[6];

    // Ponteiro para o barramento de memoria
    MMU *mmu;
}CPU;

void cpu_init(CPU *cpu, MMU *mmu);
void cpu_reset(CPU *cpu);

// helpers para leitura e alteração de flags do CPSR
bool cpu_get_flag(CPU *cpu, uint32_t flag);
void cpu_set_flag(CPU *cpu, uint32_t flag, bool value);

#endif
