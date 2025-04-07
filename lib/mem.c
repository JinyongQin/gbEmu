#include "mem.h"
#include "emu.h"
#include "cart.h"
#include "ppu.h"
#include "timer.h"
#include "io.h"
extern Emulator *emu;
// wram
inline static uint8_t wram_read(uint16_t addr)
{
    return emu->wram[addr - WRAM_START];
}
inline static void ram_write(uint16_t addr, uint8_t val)
{
    emu->wram[addr - WRAM_START] = val;
}
// vram
inline static uint8_t vram_read(uint16_t addr)
{
    return emu->ppu.vram[addr - VRAM_START];
}
inline static void vram_write(uint16_t addr, uint8_t val)
{
    emu->ppu.vram[addr - VRAM_START] = val;
}
inline static void hram_write(uint16_t addr, uint8_t val)
{
    emu->hram[addr - HRAM_START] = val;
}

inline static uint8_t hram_read(uint16_t addr)
{
    return emu->hram[addr - HRAM_START];
}

inline static void ie_write(uint16_t addr, uint8_t val)
{
    // write to ie

    emu->IE = val;
}

inline static uint8_t ie_read(uint16_t addr)
{
    // read from ie
    return emu->IE;
}

int dma_transfering(uint16_t addr)
{
    return 0;
}

// oam
inline static void oam_write(uint16_t addr, uint8_t val)
{
    emu->ppu.oam[addr - OAM_START] = val;
}

inline static uint8_t oam_read(uint16_t addr)
{
    return emu->ppu.oam[addr - OAM_START];
}

uint8_t memory_read(uint16_t addr)
{
    if (addr <= ROM_BANK_01_END)
    {
        return cart_read(addr);
    }
    else if (addr <= VRAM_END)
    {
        return vram_read(addr);
    }
    else if (addr <= EX_RAM_END)
    {
        return cart_read(addr);
    }
    else if (addr <= WRAM_BANK_END)
    {
        return wram_read(addr);
    }
    else if (addr <= ECHO_RAM_END)
    {
        printf("prohibited read from echo ram\n");
        return 0;
    }
    else if (addr <= OAM_END)
    {
        if (dma_transfering(addr))
        {
            return 0xFF; // dma transfering
        }
    }
    else if (addr <= PROHIBITED_END)
    {
        return 0xFF; // prohibited
    }
    else if (addr <= IO_END)
    {
        return io_read(addr);
    }
    else if (addr == IE_START)
    {
        return ie_read(addr);
    }
    else
    {
        return hram_read(addr);
    }
    return 0xFF; // default
}

void memory_write(uint16_t addr, uint8_t val)
{
    if (addr <= ROM_BANK_01_END)
    {
        cart_write(addr, val);
    }
    else if (addr <= VRAM_END)
    {
        vram_write(addr, val);
    }
    else if (addr <= EX_RAM_END)
    {
        exram_write(addr, val);
    }
    else if (addr <= WRAM_BANK_END)
    {
        ram_write(addr, val);
    }
    else if (addr <= ECHO_RAM_END)
    {
        printf("prohibited write to echo ram\n");
    }
    else if (addr <= OAM_END)
    {
        if (dma_transfering(addr))
        {
            return; // dma transfering
        }
        oam_write(addr, val);
    }
    else if (addr <= PROHIBITED_END)
    {
        printf("prohibited write to prohibited area\n");
        return;
    }
    else if (addr <= IO_END)
    {
        io_write(addr, val);
    }
    else if (addr == IE_START)
    {
        ie_write(addr, val);
    }
    else
    {
        hram_write(addr, val);
    }
}
