#include "common.h"
#include "emu.h"
#include "debug.h"

// #define DBG_WIN

#ifdef DEBUG
FILE *log_file = NULL;
#endif

Emulator *emu;
// get rom file from command line
// usage : ./gbemu rom.gb
// rom.gb is the gameboy rom file
int get_rom(int argc, char *argv[], FILE **rom_fp)
{
    if (argc < 2)
    {
        printf("Usage: %s rom.gb\n", argv[0]);
        return -1;
    }
    FILE *fp = fopen(argv[1], "rb"); // rom文件加载只需要一次，为方便移植，使用标准C库函数
    if (fp == NULL)
    {
        printf("Error: could not open file %s\n", argv[1]);
        return -1;
    }
    *rom_fp = fp;
    printf("Opened file: %s\n", argv[1]);
    return 0;
}

int main(int argc, char *argv[])
{
#ifdef DEBUG
    printf("Debug mode\n");
    if (!log_file)
    {
        log_file = fopen("cpu_log.txt", "w"); // 打开日志文件
        if (!log_file)
        {
            perror("Failed to open log file");
            return -1;
        }
    }
#endif

#ifdef DBG_WIN
    SDL_Event event;
    debugWin myWin = {
        .width = 800,
        .height = 600,
        .x = SDL_WINDOWPOS_CENTERED,
        .y = SDL_WINDOWPOS_CENTERED};

    // 初始化窗口
    if (init_debugWindow(&myWin) != 0)
    {
        return -1;
    }

    set_debugText(&myWin, "test window");
#endif
    FILE *rom_fp;
    // get rom file from command line
    if (get_rom(argc, argv, &rom_fp) != 0)
    {
        return -1;
    }
    emu = emu_create(rom_fp);
    // initialize emulator
    if (emu_init(emu, rom_fp) != 0)
    {
        printf("Error: could not initialize emulator\n");
        return -1;
    }
    // run emulator
    emu->running = true;
    while (emu->running)
    {
        if (emu->paused)
        {
            // delay 100ms
            SDL_Delay(100);
            continue;
        }
        emu_step(emu);
#ifdef DBG_WIN
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                emu->running = 0;
        }
        // 从串口更新文本
        blend_serialText(&myWin);
        // 更新窗口
        update_debugWindow(&myWin);
        SDL_Delay(16); // ~60 FPS
#endif
    }
#ifdef DBG_WIN
    destroy_debugWindow(&myWin);
#endif
    // destroy emulator
    emu_destroy(emu);
    return 0;
}