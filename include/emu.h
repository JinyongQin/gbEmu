#pragma once
#include "common.h"
#include "mem.h"
#include "cpu.h"
#include "cart.h"
#include "timer.h"
#include "serial.h"
#include "ppu.h"
typedef struct Emulator
{
    CPU cpu;   // one CPU
    Cart cart; // one cartridge
    PPU ppu;   // one PPU
    // ...
    reg8 IF; // interrupt flag
    reg8 IE; // interrupt
    Timer timer;
    Serial serial; // serial
    // ram:
    uint8_t *wram; // 4+4KB Work RAM (WRAM)
    uint8_t *hram; // 127 bytes High RAM (HRAM)
    uint8_t *wave_ram;
    bool running;
    bool paused;
    float time_scale;      // time scale
    uint64_t clock_cycles; // clock cycles
    void (*tick_e)(struct Emulator *emu, uint64_t m_ticks);
} Emulator;
// create emulator
Emulator *emu_create(FILE *rom_fp);

// initialize emulator
int emu_init(Emulator *emu, FILE *rom_fp);
// destroy emulator
void emu_destroy(Emulator *emu);

void emu_step(Emulator *emu);
int emu_reset(Emulator *emu);
void emu_stop(Emulator *emu);
