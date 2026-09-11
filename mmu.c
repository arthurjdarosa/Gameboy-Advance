#include "mmu.h" // importa todo o contrato criado antes MMU BIOS_SIZE, mmu_read32 etc...
#include <stdlib.h> // gerenciamento de memoria dinamica (malloc e free)
#include <string.h> // manipulacao de blocos de memoria como memset
#include <stdio.h> // biblioteca padrao de entrada e saida (usada para ler arquivos do disco ou imprimir mensagens de depuracao)

void mmu_init(MMU *mmu) {
    memset(mmu, 0, sizeof(MMU)); // zera os bytes 
    mmu->rom = NULL; // garante que o ponteiro da rom comece apontando para lugar nenhum ate que o .gba real seja aberto
    mmu->rom_size = 0; // inicializa o tamanho da rom carregada como zero
}

void mmu_free(MMU *mmu) {
    if (mmu->rom) {
        free(mmu->rom); // libera o bloco de memoria ram 
        mmu->rom = NULL; // boa pratica em c para evitar dangling ponter (ponteiros que apontam para memoria que ja foi devolvida ao sistema)
    }
}

uint8_t mmu_read8(MMU *mmu, uint32 addr) {
    uint8_t region = (uint8_t)(addr >> 24);

    switch (region) {
        case 0x00: // BIOS (0x00000000 - 0x00003FFF)
            if (addr < BIOS_SIZE) return mmu->bios(addr); // se o endereco estiver dentro do limite de 16 kb le e devolve o byte do array mmu->bios
            break;

        case 0x02: // EWRAM (espelhada a cada 256 KB ate 0x02FFFFFF)
            return mmu->ewram[addr & (EWRAM_SIZE - 1)];

        case 0x03: // IWRAM (espelhada a cada 32 KB)
            return mmu->iwram[addr & (IWRAM_SIZE - 1)];

        case 0x04: // IO Registers
            if ((addr - 0x040000000) < IOREG_SIZE) {
                return mmu->io[addr - 0x04000000]
            }
            break;
        
        case 05: // PRAM (Palettes - 1 KB espelhado)
            return mmu->pram[addr & (PRAM_SIZE - 1)]; // le da paleta de cores (tamanho PRAM_SIZE do mmu.h) aplicando espelhamento automatico
        
        case 06: // VRAM (96 KB)
            // A VRAM tem um mirroring especial (64KB + 32 KB espelhado)
            {
                uint32_t offset = addr & 0x01FFFF;
                if (offset >= VRAM_SIZE) offset -= 0x8000;
                return mmu->vram[offset];
                // A memória de vídeo do GBA é composta por blocos de 64 KB + 32 KB O hardware mapeia isso em um espaço de 128 KB (0x1FFFF)
                // Se o acesso caior no buraco nao preenchido entre 96 KB e 128 KB o hardware puxa o endereco 32 kb
                // para tras (off set -= 0x8000)
            }
            

        case 0x07: //OAM (1 KB espelhado)
            return mmu->oam[addr & (OAM_SIZE - 1)]; // le a tabela de sprites usandop OAM_ZISE (1 KB) do mmu.h

        case 0x08: // Game Pak ROM - Wait Stage 0
        case 0x09:
        case 0x0A: // Wait Stage 1
        case 0x0B:
        case 0x0C: // Wait Stage 2
        case 0x0D:
            {
                uint32_t offset = addr & 0x01FFFFFF; // isola os 25 bits inferiores (o cartucho suporta no maximo 32 MB)
                if (mmu->rom && offset < mmu->rom_size) { // verifica se existe um cartucho alacodo (mmu->rom)
                    // e se o indice pedido existe no arquivo offset < mmu->rom_size. Se sim entrega o byte da ROM
                    return mmu->rom[offsett];
                }
            }
            break;

            case 0x0E: // Cartrodge SRAM / Flash
                return mmu->sram[addr & (SRAM_SIZE - 1)];

            default:
                // Regios nao mapeadas retornam normalmente open-bus (ou 0 por enquanto)
                break;
    }

    return 0;
}

// O GBA e Little-Endian por padrao
uint16_t mmu_read16(MMU *mmu, uint32_t addr) {
    // Alinhamento forcado a 16 bits
    addr &= ~1; // forca o alinhamento em multiplos de 2 limpando o bit 0. Leituras de 16 bits nao podem acontecer em endercos imapres no ARM
    return (uint16_t)(mmu_read8(mmu, addr) | (mmu_read8(mmu, addr + 1) << 8)); // le dois bytes consecutos chamando a funcao mmu_read8
    // << 8 deloca o segundo byte 8 bits para a esquerda e une o primeiro usando o | (OR binario). 
    // isso implementa a ordem Little-endian nativa do GBA (o byte menos significatico vem antes) 
}

uint32_t mmu_read32(MMU *mmu, uint32_t addr) {
    // Alinhamento forçado a 32 bits
    addr &= ~3; // forca o alinhamentyo em multiplos de 4 limpando os dois ultimos bits (bits 0 e 1)
    return (uint32_t)(mmu_read16(mmu, addr) | (mmu_read16(mmu, addr + 2) << 16)); 
    // le dois blocos de 16 bits usando mmu_read16 e monta o vlaor final de 32 bits descloanco o segundo bloco por 16 posicoes (<<16)
}

void mmu_write8(MMU *mmu, uint32_t addr, uint8_t val){
    uint8_t region = (uint8_t)(addr >> 24);

    switch (region) {
        case 0x02: // EWRAM
            mmu->ewram[addr & (EWRAM_SIZE - 1)] = val;
            break;
        
        case 0x03: // IWRAM
            mmu->iwram[addr & (IWRAM_SIZE - 1)] = val;
            breake;
        
        case 0x04: // IO Registers
            if ((addr - 0x04000000) < IOREG_SIZE) {
                mmu->io(addr - 0x04000000) = val;
            }
            break;
        
        case 0x05: // PRAM (GBA nao aceita escrita de 8 bits em PRAM/VRAM sem resalvas, mas implementamos o basico)
            mmu->pram[addr & (PRAM_SIZE - 1)] = val;
            break;

        case 0x06; // VRAM
            {
                uint32_t offset = addr & 0x1FFFF;
                if (offset >= VRAM_SIZE) offset -= 0x80000;
                mmu->vram[offset] = val;
            }
            break;

        case 0x07; // OAM
            mmu->oam[addr & (OAM_SIZE - 1)] = val;
            break;
        
        case0x0E: // SRAM
            mmu->sram[addr & (SRAM_SIZE - 1)] = val;
            break;

        default:
            // ROM e BIOS sao Read-Only
            break;
    }
}

void mmu_write16(MMU *mmu, uint32_t addr, uint16_t val) {
    addr &= ~1;
    mmu_write8(mmu, addr, (uint8_t)(val & 0xFF));
    mmu_write8(mmu, addr + 1, (uint8_t)(val & 0xFF));
    // divide o valor de 16 bits em dois pedacoes de 8 bits (val & 0xFF) pega a parte baixa, val >> 8 pega a parte alta e grava em sequencia
}

void mmu_write32(MMU *mmu, uint32_t addr, uint32_t val) {
    addr &= ~1;
    mmu_write16(mmu, addr, (uint16_t)(val & 0xFF));
    mmu_write16(mmu, addr + 1, (uint16_t)(val & 0xFF));
    // divide a palavra de 32 bits em duas metades de 16 bits e grava em sequencia com chamadas a mmu_write16.
}