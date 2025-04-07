#pragma once
#include "common.h"
#define IO_START 0xFF00
#define IO_INDEX(addr) ((addr) - IO_START)

typedef uint8_t (*io_read_func)();
typedef void (*io_write_func)(uint8_t val);
typedef struct io_handler
{
    io_read_func read;
    io_write_func write;
} IOHandler;

void io_write(uint16_t addr, uint8_t val);
uint8_t io_read(uint16_t addr);