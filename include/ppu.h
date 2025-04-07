#pragma once
#include "common.h"
#include "emu.h"

#define LINES_PER_FRAME 154 // 154 lines per frame
#define CYCLES_PER_LINE 456 // 456 cycles per line

#define XRESOLUTION 160
#define YRESOLUTION 144

#define PPU_REG_START 0xFF40 // PPU register start
#define PPU_REG_LCDC 0xFF40  // PPU register LCDC
#define PPU_REG_STAT 0xFF41  // PPU register STAT
#define PPU_REG_SCY 0xFF42   // PPU register SCY
#define PPU_REG_SCX 0xFF43   // PPU register SCX
#define PPU_REG_LY 0xFF44    // PPU register LY
#define PPU_REG_LYC 0xFF45   // PPU register LYC
#define PPU_REG_DMA 0xFF46   // PPU register DMA
#define PPU_REG_BGP 0xFF47   // PPU register BGP
#define PPU_REG_OBP0 0xFF48  // PPU register OBP0
#define PPU_REG_OBP1 0xFF49  // PPU register OBP1
#define PPU_REG_WY 0xFF4A    // PPU register WY
#define PPU_REG_WX 0xFF4B    // PPU register WX
// ppu mode enum
typedef enum
{
    HBLANK = 0,     // H-Blank
    VBLANK = 1,     // V-Blank
    OAM_SCAN = 2,   // OAM Scan
    PIXEL_DRAW = 3, // Pixel Draw
} PPU_MODE;

#define HBLANK_CYCLES 204 // H-Blank cycles
#define VBLANK_CYCLES 456 // V-Blank cycles
#define OAM_SCAN_CYCLES 80
#define PIXEL_DRAW_CYCLES 172 // Pixel Draw cycles
// ppu mode handler
typedef void (*PPU_MODE_HANDLER)(void);

// ppu struct
typedef struct PPU
{
    reg8 lcdc;            // LCD control - 0xFF40
    reg8 lcds;            // LCD status - 0xFF41
    reg8 scy;             // scroll y - 0xFF42
    reg8 scx;             // scroll x - 0xFF43
    reg8 ly;              // line y - 0xFF44
    reg8 lyc;             // line y compare - 0xFF45
    reg8 dma;             // DMA transfer - 0xFF46
    reg8 bgp;             // background palette - 0xFF47
    reg8 obp0;            // object palette 0 - 0xFF48
    reg8 obp1;            // object palette 1 - 0xFF49
    reg8 wy;              // window y - 0xFF4A
    reg8 wx;              // window x - 0xFF4B
    uint8_t *vram;        // 8KB Video RAM (VRAM)
    uint8_t *oam;         // Object attribute memory (OAM)
    uint32_t line_cycles; // 当前行所经过的时钟周期
} PPU;

#define PPU_ENABLED(ppu) ((ppu).lcdc & 0x80)
#define PPU_MODE(ppu) ((ppu).lcdc & 0x03)
#define PPU_SET_MODE(ppu, mode) ((ppu).lcdc = ((ppu).lcdc & 0xFC) | (mode))
// set 1 to ly bit 2
#define SET_LY_FLAG(ppu) ((ppu).ly |= 0x04)   // ly |= 0b00000100
#define CLEAR_LY_FLAG(ppu) ((ppu).ly &= 0xFB) // ly &= 0b11111011

#define HBLANK_IT_ENABLED(ppu) ((ppu).lcdc & 0x20)
#define VBLANK_IT_ENABLED(ppu) ((ppu).lcdc & 0x10)
#define OAM_IT_ENABLED(ppu) ((ppu).lcdc & 0x08)
#define LYC_IT_ENABLED(ppu) ((ppu).lcdc & 0x04)

void PPU_init(PPU *ppu);
void PPU_tick(PPU *ppu);
void PPU_destroy(PPU *ppu);
