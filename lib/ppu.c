#include "emu.h"
#include "ppu.h"
#include "ir.h"
extern Emulator *emu;

#define _PPU (emu->ppu)

#define LY (_PPU.ly)
#define LYC (_PPU.lyc)
#define LCDC (_PPU.lcdc)
#define LCDS (_PPU.lcds)
#define SCY (_PPU.scy)
#define SCX (_PPU.scx)
#define DMA (_PPU.dma)
#define BGP (_PPU.bgp)
#define OBP0 (_PPU.obp0)
#define OBP1 (_PPU.obp1)
#define WY (_PPU.wy)
#define WX (_PPU.wx)

void PPU_init(PPU *ppu)
{
    ppu->lcdc = 0x91;
    ppu->lcdc = 0x00;
    ppu->scy = 0x00;
    ppu->scx = 0x00;
    ppu->ly = 0x00;
    ppu->lyc = 0x00;
    ppu->dma = 0x00;
    ppu->bgp = 0xFC;
    ppu->obp0 = 0xFF;
    ppu->obp1 = 0xFF;
    ppu->wy = 0x00;
    ppu->wx = 0x00;
    ppu->line_cycles = 0;
    // allocate memory for vram and oam
    ppu->vram = (uint8_t *)malloc(VRAM_SIZE);
    if (ppu->vram == NULL)
    {
        printf("Error: could not allocate memory for vram\n");
        return;
    }
    ppu->oam = (uint8_t *)malloc(OAM_SIZE);
    if (ppu->oam == NULL)
    {
        printf("Error: could not allocate memory for oam\n");
        free(ppu->vram);
        return;
    }
    // set default mode
    PPU_SET_MODE(*ppu, OAM_SCAN);
}
void PPU_destroy(PPU *ppu)
{
    if (ppu->vram != NULL)
    {
        free(ppu->vram);
        ppu->vram = NULL;
    }
    if (ppu->oam != NULL)
    {
        free(ppu->oam);
        ppu->oam = NULL;
    }
}

static inline void inc_ly(void)
{
    LY++;
    if (LY == LYC)
    {
        SET_LY_FLAG(_PPU);
        if (LYC_IT_ENABLED(_PPU))
        {
            emu->IF |= IR_STAT_MASK; // set interrupt flag
        }
    }
    else
    {
        CLEAR_LY_FLAG(_PPU);
    }
}

static void mode_hblank_handel()
{
    if (_PPU.line_cycles >= CYCLES_PER_LINE)
    {
        inc_ly();
        if (LY >= YRESOLUTION)
        {
            PPU_SET_MODE(_PPU, VBLANK);
            // interupt
            emu->IF |= IR_VBLANK_MASK; // set interrupt flag
            if (VBLANK_IT_ENABLED(_PPU))
            {
                emu->IF |= IR_STAT_MASK; // set interrupt flag
            }
        }
        else
        {
            PPU_SET_MODE(_PPU, OAM_SCAN);
            if (LYC_IT_ENABLED(_PPU))
            {
                emu->IF |= IR_STAT_MASK; // set interrupt flag
            }
        }
        _PPU.line_cycles = 0;
    }
}

static void mode_vblank_handel()
{
    if (_PPU.line_cycles >= CYCLES_PER_LINE)
    {
        inc_ly();
        if (LY >= LINES_PER_FRAME)
        {
            PPU_SET_MODE(_PPU, OAM_SCAN);
            LY = 0;
            if (OAM_IT_ENABLED(_PPU))
            {
                emu->IF |= IR_STAT_MASK; // set interrupt flag
            }
        }
        _PPU.line_cycles = 0;
    }
}
static void mode_oam_scan_handel()
{
    // todo...
    if (_PPU.line_cycles >= 80)
    {
        PPU_SET_MODE(_PPU, PIXEL_DRAW);
    }
}
static void mode_pixel_draw_handel()
{
    // todo ...
    if (_PPU.line_cycles >= 369)
    {
        PPU_SET_MODE(_PPU, HBLANK);
        if (HBLANK_IT_ENABLED(_PPU))
        {
            emu->IF |= IR_STAT_MASK; // set interrupt flag
        }
    }
}

PPU_MODE_HANDLER ppu_mode_handler_table[4] = {
    [HBLANK] = mode_hblank_handel,
    [VBLANK] = mode_vblank_handel,
    [OAM_SCAN] = mode_oam_scan_handel,
    [PIXEL_DRAW] = mode_pixel_draw_handel,
};

void PPU_tick(PPU *ppu)
{
    if (PPU_ENABLED(*ppu) == 0)
    {
        return;
    }
    ppu->line_cycles++;
    ppu_mode_handler_table[PPU_MODE(*ppu)]();
}
