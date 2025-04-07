#include "emu.h"
#include <string.h>

#define RAM_INIT(r, R)                                           \
    emu->r = (uint8_t *)malloc(R##_SIZE);                        \
    if (emu->r == NULL)                                          \
    {                                                            \
        printf("Error: could not allocate memory for " #r "\n"); \
        return -1;                                               \
    }                                                            \
    memset(emu->r, 0, R##_SIZE);
#define RAM_FREE(r)     \
    if (emu->r != NULL) \
    {                   \
        free(emu->r);   \
        emu->r = NULL;  \
    }

void tick_e(Emulator *emu, uint64_t mticks)
{
    // gameboy clock is 4MHz
    uint32_t num_tick = mticks * 4; // 4 ticks per mtick
    for (uint32_t i = 0; i < num_tick; i++)
    {
        ++emu->clock_cycles;
        timer_tick(&(emu->timer));
        // serial clk : 8192Hz
        if (emu->clock_cycles % 512 == 0)
        {
            serial_tick(&(emu->serial));
        }
        PPU_tick(&(emu->ppu));
        // to do: other sync
    }
}

int emu_init(Emulator *emu, FILE *rom_fp)
{
    printf("init emu\n");
    emu->running = false;
    emu->paused = false;
    emu->clock_cycles = 0;
    emu->time_scale = 1.0f;
    emu->tick_e = tick_e;
    emu->IE = 0;
    emu->IF = 0;
    // cpu init
    emu->cpu = (CPU){0};
    if (cpu_init(&emu->cpu))
    {
        printf("Error: cpu init failed\n");
        return -1;
    }
    // ram init
    RAM_INIT(wram, WRAM);
    RAM_INIT(hram, HRAM);
    RAM_INIT(wave_ram, WAVE_RAM);
    // timer init
    timer_init(&emu->timer);
    // PPU init
    PPU_init(&emu->ppu);
    // serial init
    serial_init(&emu->serial);
    // create cartridge
    emu->cart.rom_fp = rom_fp;
    // load rom
    if (cart_load_rom(&emu->cart) != 0)
    {
        printf("Error: could not load rom\n");
        return -1;
    }
    return 0;
}

inline void emu_step(Emulator *emu)
{
    // cpu step
    emu->cpu.step(&emu->cpu);
}

void emu_pause(Emulator *emu)
{
    printf("pause emu\n");
}

void emu_stop(Emulator *emu)
{
    emu->running = false;
    emu->paused = false;

    // remove cartridge

    printf("stop emu\n");
}

int emu_reset(Emulator *emu)
{
    printf("reset emu\n");
    return 0;
}

Emulator *emu_create(FILE *rom_fp)
{
    Emulator *emu = (Emulator *)malloc(sizeof(Emulator));
    if (emu == NULL)
    {
        printf("Error: could not allocate memory for emulator\n");
        return NULL;
    }
    return emu;
}
void emu_destroy(Emulator *emu)
{
    // free ram
    RAM_FREE(wram);
    RAM_FREE(hram);
    RAM_FREE(wave_ram);

    // free cartridge
    cart_destroy(&emu->cart);
    // free emulator
    if (emu != NULL)
    {
        free(emu);
        emu = NULL;
    }
}
