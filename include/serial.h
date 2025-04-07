#pragma once

#include "common.h"
typedef struct Serial // out only
{
    reg8 sb;
    reg8 sc;
    uint8_t out_byte;
    uint8_t out_bit; // index 0-7
    bool transfering;
} Serial;

void serial_init(Serial *s);
void serial_tick(Serial *s);
