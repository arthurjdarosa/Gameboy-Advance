#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <stddef.h>

// tamanhos reais da memoria do gba
// contantantes pre-compiladas, defindo tamanhos exatos de cada chip interno em bytes
#define BIOS_SIZE (16 * 1024) // 16 KB - em bytes 16.384
#define EWRAM_SIZE (256 * 1024) // 256 KB - em bytes 262.144
#define IWRAM_SIZE (32 *1024) // 32 KB - em bytes 32.768
#define IOREG_SIZE (1024) // 1 KB (0x04000000 - 0x040003FF) - em bytes 1024
#define PRAM_SIZE (1024) // 1 KB (Pallettes) - em bytes 1024
#define VRAM_SIZE (96 * 1024) //96 KB - em bytes 98.304
#define OAM_SIZE (1024) // 1 KB - em bytes 1024
#define ROM_MAX_SIZE (32 * 1024 * 1024) // ate 32 MB - em bytes 33.554.432
#define SRAM_SIZE (64 * 1024) // 64 KB (Flash / SRAM para Saves) - em bytes 64.536

// Cria a struct e batizaq de MMU 
typedef struct {
    uint8_t bios[BIOS_SIZE]; // Um array contiguo de 16.384 bytes para a BIOS
    uint8_t ewram[EWRAM_SIZE]; // Arrays estaticos de byte que modelam as memorias internas e registradores
    uint8_t iwram[IWRAM_SIZE]; 
    uint8_t io[IOREG_SIZE];
    uint8_t pram[PRAM_SIZE];
    uint8_t vram[VRAM_SIZE];
    uint8_t oam[OAM_SIZE];
    uint8_t *rom; // ponteiro de memoria dinamica por conta que um cartucho de GBA pode ter 4, 8, 16 ou 32 MB, 
    // usamos um ponteiro malloc apenas o tamanho exato da ROM aberta
    size_t rom_size; // Variavel numeria para guardar quantos bytes o arquivo carregado possui
    uint8_t sram[SRAM_SIZE]; // Memória de salvamento do cartucho
} MMU;

// Assinaturas de ciclo de vida declara que existem as funcoes mmu_init (zera a memoria e inicializa ponteitos) e 
// o mmu_free (libera a memoria alocada da ROM quando o emulador fecha)
void mmu_init(MMU *mmu);
void mmu_free(MMU *mmu);

// Leituras com suporte a alinhamento de 8, 16 e 32 bits
// preciso ler uma inbstrucao de 4 bytes que esta no endereco 0x08000000 ela chama mmu_read32 mmu, 0x08000000
// a funcao analisa o endereco 0x08... e regiacao de cartucho ROM vou la no array mmu -> rom, pego os 4 bytes seguidos 
// monto um numero de 32 bits e devolvo para CPU
uint8_t mmu_read8(MMU *mmu, uint32_t addr); 
uint16_t mmu_read16(MMU *mmu, uint32_t addr);
uint32_t mmu_read32(MMU *mmu, uint32_t addr);

// Escritas 
// O jogo quer mudar a vida do personagem na memoria e manda gravar o valor 999 (2 Bytes) no endereco 0x03001200 
// ele chama o mmu_white16(mmu, 0x03001200, 999) 
// a funcao inspeciona o endereco "opa 0x03... e a IWRAM vou gravar o valor 999 dentro do array mmu_iwram no indice correto"
void mmu_write8(MMU *mmu, uint32_t addr, uint8_t val);
void mmu_write16(MMU *mmu, uint32_t addr, uint16_t val);
void mmu_write32(MMU *mmu, uint32_t addr, uint32_t val);

// fecha o bloco condicionado aberto la na primeira linha
#endif