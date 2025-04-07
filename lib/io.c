#include "io.h"
#include "emu.h"
#include "ppu.h"
extern Emulator *emu;
#define PPU_REG_WRITE(reg)              \
    void PPU_##reg##_write(uint8_t val) \
    {                                   \
        emu->ppu.reg = val;             \
    }

#define PPU_REG_READ(reg)      \
    uint8_t PPU_##reg##_read() \
    {                          \
        return emu->ppu.reg;   \
    }
void PPU_lcdc_write(uint8_t val)
{
    if (PPU_ENABLED(emu->ppu) == 0 && (val & 0x80) == 0)
    {
        emu->ppu.lcds = 0x7C; // 0b01111100
        emu->ppu.ly = 0x00;
        emu->ppu.line_cycles = 0; // 重新开始计时
    }
    emu->ppu.lcdc = val;
}
void PPU_lcds_write(uint8_t val)
{
    // 0xFF41的低三位为只读
    val &= 0xF8;
    // 0xFF41的高三位为可读写
    val |= (emu->ppu.lcds & 0x07);
}
PPU_REG_WRITE(scy);
PPU_REG_WRITE(scx);
PPU_REG_WRITE(ly);
PPU_REG_WRITE(lyc);
PPU_REG_WRITE(dma);
PPU_REG_WRITE(bgp);
PPU_REG_WRITE(obp0);
PPU_REG_WRITE(obp1);
PPU_REG_WRITE(wy);
PPU_REG_WRITE(wx);
PPU_REG_READ(lcdc);
PPU_REG_READ(lcds);
PPU_REG_READ(scy);
PPU_REG_READ(scx);
PPU_REG_READ(ly);
PPU_REG_READ(lyc);
PPU_REG_READ(dma);
PPU_REG_READ(bgp);
PPU_REG_READ(obp0);
PPU_REG_READ(obp1);
PPU_REG_READ(wy);
PPU_REG_READ(wx);
// no define read
static uint8_t no_define_read()
{
    printf("error : io addr no defined!\n");
    return 0xFF;
}
static void no_define_write(uint8_t val)
{
    printf("error : io addr no defined!\n");
}

// serial sb register
static uint8_t sb_read()
{
    return emu->serial.sb;
}

static void sb_write(uint8_t val)
{
    emu->serial.sb = val;
}

// serial sc register
static uint8_t sc_read()
{
    return emu->serial.sc;
}

static void sc_write(uint8_t val)
{
    // 0x7C = 01111100
    // 0x83 = 10000011
    emu->serial.sc = 0x7C | (val & 0x83);
}

// div register
static uint8_t div_read()
{
    return emu->timer.DIV;
}
static void div_write(uint8_t val)
{
    // DIV register is read-only, writing to it has no effect
    // but we need to reset the timer
    emu->timer.DIV = 0;
}
// tima register
static uint8_t tima_read()
{
    return emu->timer.TMC;
}
static void tima_write(uint8_t val)
{
    emu->timer.TMC = val;
}
// tma register
static uint8_t tma_read()
{
    return emu->timer.TMA;
}
static void tma_write(uint8_t val)
{
    emu->timer.TMA = val;
}
// tac register
static uint8_t tac_read()
{
    return emu->timer.TAC;
}
static void tac_write(uint8_t val)
{
    emu->timer.TAC = val;
    // 00 = 9, 01 = 3, 10 = 5, 11 = 7
    // set div update bit mask
    switch (val & 0x03)
    {
    case 0x00:
        emu->timer.div_update_bit_mask = DIV_BIT_MASK(9);
        break;
    case 0x01:
        emu->timer.div_update_bit_mask = DIV_BIT_MASK(3);
        break;
    case 0x02:
        emu->timer.div_update_bit_mask = DIV_BIT_MASK(5);
        break;
    case 0x03:
        emu->timer.div_update_bit_mask = DIV_BIT_MASK(7);
    }
    // update timer enable
    if (val & 0x04)
    {
        emu->timer.enabled = true;
    }
    else
    {
        emu->timer.enabled = false;
    }
}
// IF register
static uint8_t if_read()
{
    return emu->IF;
}
static void if_write(uint8_t val)
{
    emu->IF = val;
}
// wave ram
static uint8_t wave_ram_read(uint16_t addr)
{
    return emu->wave_ram[addr - 0xFF30];
}
static void wave_ram_write(uint16_t addr, uint8_t val)
{
    emu->wave_ram[addr - 0xFF30] = val;
}
static const IOHandler mapping[0x80] = {
    /* 0x00 */ {no_define_read, no_define_write}, // 0xFF00 对应 Joypad (P1/JOYP)
    /* 0x01 */ {sb_read, sb_write},               // 0xFF01 对应 SB
    /* 0x02 */ {sc_read, sc_write},               // 0xFF02 对应 SC
    /* 0x03 */ {no_define_read, no_define_write}, // 0xFF03 未定义
    /* 0x04 */ {div_read, div_write},             // 0xFF04 对应 DIV
    /* 0x05 */ {tima_read, tima_write},           // 0xFF05 对应 TIMA
    /* 0x06 */ {tma_read, tma_write},             // 0xFF06 对应 TMA
    /* 0x07 */ {tac_read, tac_write},             // 0xFF07 对应 TAC
    /* 0x08 */ {no_define_read, no_define_write}, // 0xFF08 无效
    /* 0x09 */ {no_define_read, no_define_write},
    /* 0x0A */ {no_define_read, no_define_write},
    /* 0x0B */ {no_define_read, no_define_write},
    /* 0x0C */ {no_define_read, no_define_write},
    /* 0x0D */ {no_define_read, no_define_write},
    /* 0x0E */ {no_define_read, no_define_write},
    /* 0x0F */ {if_read, if_write},               // 0xFF0F 对应 IF
    /* 0x10 */ {no_define_read, no_define_write}, // 0xFF10 NR10
    /* 0x11 */ {no_define_read, no_define_write}, // 0xFF11 NR11
    /* 0x12 */ {no_define_read, no_define_write}, // 0xFF12 NR12
    /* 0x13 */ {no_define_read, no_define_write}, // 0xFF13 NR13
    /* 0x14 */ {no_define_read, no_define_write}, // 0xFF14 NR14
    /* 0x15 */ {no_define_read, no_define_write},
    /* 0x16 */ {no_define_read, no_define_write}, // 0xFF16 NR21
    /* 0x17 */ {no_define_read, no_define_write}, // 0xFF17 NR22
    /* 0x18 */ {no_define_read, no_define_write}, // 0xFF18 NR23
    /* 0x19 */ {no_define_read, no_define_write}, // 0xFF19 NR24
    /* 0x1A */ {no_define_read, no_define_write}, // 0xFF1A NR30
    /* 0x1B */ {no_define_read, no_define_write}, // 0xFF1B NR31
    /* 0x1C */ {no_define_read, no_define_write}, // 0xFF1C NR32
    /* 0x1D */ {no_define_read, no_define_write}, // 0xFF1D NR33
    /* 0x1E */ {no_define_read, no_define_write}, // 0xFF1E NR34
    /* 0x1F */ {no_define_read, no_define_write},
    /* 0x20 */ {no_define_read, no_define_write}, // 0xFF20 NR41
    /* 0x21 */ {no_define_read, no_define_write}, // 0xFF21 NR42
    /* 0x22 */ {no_define_read, no_define_write}, // 0xFF22 NR43
    /* 0x23 */ {no_define_read, no_define_write}, // 0xFF23 NR44
    /* 0x24 */ {no_define_read, no_define_write}, // 0xFF24 NR50
    /* 0x25 */ {no_define_read, no_define_write}, // 0xFF25 NR51
    /* 0x26 */ {no_define_read, no_define_write}, // 0xFF26 NR52
    /* 0x27 */ {no_define_read, no_define_write},
    /* 0x28 */ {no_define_read, no_define_write},
    /* 0x29 */ {no_define_read, no_define_write},
    /* 0x2A */ {no_define_read, no_define_write},
    /* 0x2B */ {no_define_read, no_define_write},
    /* 0x2C */ {no_define_read, no_define_write},
    /* 0x2D */ {no_define_read, no_define_write},
    /* 0x2E */ {no_define_read, no_define_write},
    /* 0x2F */ {no_define_read, no_define_write},
    /* 0x30 */ {no_define_read, no_define_write}, // 0xFF30-0xFF3F WAVE RAM
    /* 0x31 */ {no_define_read, no_define_write},
    /* 0x32 */ {no_define_read, no_define_write},
    /* 0x33 */ {no_define_read, no_define_write},
    /* 0x34 */ {no_define_read, no_define_write},
    /* 0x35 */ {no_define_read, no_define_write},
    /* 0x36 */ {no_define_read, no_define_write},
    /* 0x37 */ {no_define_read, no_define_write},
    /* 0x38 */ {no_define_read, no_define_write},
    /* 0x39 */ {no_define_read, no_define_write},
    /* 0x3A */ {no_define_read, no_define_write},
    /* 0x3B */ {no_define_read, no_define_write},
    /* 0x3C */ {no_define_read, no_define_write},
    /* 0x3D */ {no_define_read, no_define_write},
    /* 0x3E */ {no_define_read, no_define_write},
    /* 0x3F */ {no_define_read, no_define_write},
    /* 0x40 */ {PPU_lcdc_read, PPU_lcdc_write}, // 0xFF40 LCDC
    /* 0x41 */ {PPU_lcds_read, PPU_lcds_write}, // 0xFF41 STAT
    /* 0x42 */ {PPU_scy_read, PPU_scy_write},   // 0xFF42 SCY
    /* 0x43 */ {PPU_scx_read, PPU_scx_write},   // 0xFF43 SCX
    /* 0x44 */ {PPU_ly_read, PPU_ly_write},     // 0xFF44 LY
    /* 0x45 */ {PPU_lyc_read, PPU_lyc_write},   // 0xFF45 LYC
    /* 0x46 */ {PPU_dma_read, PPU_dma_write},   // 0xFF46 DMA
    /* 0x47 */ {PPU_bgp_read, PPU_bgp_write},   // 0xFF47 BGP
    /* 0x48 */ {PPU_obp0_read, PPU_obp0_write}, // 0xFF48 OBP0
    /* 0x49 */ {PPU_obp1_read, PPU_obp1_write}, // 0xFF49 OBP1
    /* 0x4A */ {PPU_wy_read, PPU_wy_write},     // 0xFF4A WY
    /* 0x4B */ {PPU_wx_read, PPU_wx_write},     // 0xFF4B WX
    /* 0x4C */ {no_define_read, no_define_write},
    /* 0x4D */ {no_define_read, no_define_write}, // 0xFF4D KEY1
    /* 0x4E */ {no_define_read, no_define_write},
    /* 0x4F */ {no_define_read, no_define_write}, // 0xFF4F VBK
    /* 0x50 */ {no_define_read, no_define_write},
    /* 0x51 */ {no_define_read, no_define_write}, // 0xFF51 HDMA1
    /* 0x52 */ {no_define_read, no_define_write}, // 0xFF52 HDMA2
    /* 0x53 */ {no_define_read, no_define_write}, // 0xFF53 HDMA3
    /* 0x54 */ {no_define_read, no_define_write}, // 0xFF54 HDMA4
    /* 0x55 */ {no_define_read, no_define_write}, // 0xFF55 HDMA5
    /* 0x56 */ {no_define_read, no_define_write}, // 0xFF56 RP
    /* 0x57 */ {no_define_read, no_define_write},
    /* 0x58 */ {no_define_read, no_define_write},
    /* 0x59 */ {no_define_read, no_define_write},
    /* 0x5A */ {no_define_read, no_define_write},
    /* 0x5B */ {no_define_read, no_define_write},
    /* 0x5C */ {no_define_read, no_define_write},
    /* 0x5D */ {no_define_read, no_define_write},
    /* 0x5E */ {no_define_read, no_define_write},
    /* 0x5F */ {no_define_read, no_define_write},
    /* 0x60 */ {no_define_read, no_define_write},
    /* 0x61 */ {no_define_read, no_define_write},
    /* 0x62 */ {no_define_read, no_define_write},
    /* 0x63 */ {no_define_read, no_define_write},
    /* 0x64 */ {no_define_read, no_define_write},
    /* 0x65 */ {no_define_read, no_define_write},
    /* 0x66 */ {no_define_read, no_define_write},
    /* 0x67 */ {no_define_read, no_define_write},
    /* 0x68 */ {no_define_read, no_define_write}, // 0xFF68 BCPS/BGPI
    /* 0x69 */ {no_define_read, no_define_write}, // 0xFF69 BCPD/BGPD
    /* 0x6A */ {no_define_read, no_define_write}, // 0xFF6A OCPS/OBPI
    /* 0x6B */ {no_define_read, no_define_write}, // 0xFF6B OCPD/OBPD
    /* 0x6C */ {no_define_read, no_define_write}, // 0xFF6C OPRI
    /* 0x6D */ {no_define_read, no_define_write},
    /* 0x6E */ {no_define_read, no_define_write},
    /* 0x6F */ {no_define_read, no_define_write},
    // 0xFF70
    /* 0x70 */ {no_define_read, no_define_write}, // 0xFF70 SVBK
    /* 0x71 */ {no_define_read, no_define_write},
    /* 0x72 */ {no_define_read, no_define_write},
    /* 0x73 */ {no_define_read, no_define_write},
    /* 0x74 */ {no_define_read, no_define_write},
    /* 0x75 */ {no_define_read, no_define_write},
    /* 0x76 */ {no_define_read, no_define_write}, // 0xFF76 PCM12
    /* 0x77 */ {no_define_read, no_define_write}, // 0xFF77 PCM34
    /* 0x78 */ {no_define_read, no_define_write},
    /* 0x79 */ {no_define_read, no_define_write},
    /* 0x7A */ {no_define_read, no_define_write},
    /* 0x7B */ {no_define_read, no_define_write},
    /* 0x7C */ {no_define_read, no_define_write},
    /* 0x7D */ {no_define_read, no_define_write},
    /* 0x7E */ {no_define_read, no_define_write},
    /* 0x7F */ {no_define_read, no_define_write},
};

inline void io_write(uint16_t addr, uint8_t val)
{
    // wave ram acc
    if (addr >= 0xFF30 && addr <= 0xFF3F)
    {
        wave_ram_write(addr, val);
        return;
    }
    mapping[IO_INDEX(addr)].write(val);
}

inline uint8_t io_read(uint16_t addr)
{
    if (addr >= 0xFF30 && addr <= 0xFF3F)
    {
        return wave_ram_read(addr);
    }
    return mapping[IO_INDEX(addr)].read();
}