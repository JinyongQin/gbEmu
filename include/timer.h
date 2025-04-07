#pragma once

#include "common.h"

#define DIV_BIT_MASK(n) (uint16_t)(1 << (n))

typedef struct Timer
{
    reg16 DIV;                    // Timer Divider Register
    uint16_t div_update_bit_mask; // when it change from 0 to 1,update tmc
    reg8 TMA;                     // Timer Modulo Register
    reg8 TAC;                     // Timer Control Register
    reg8 TMC;                     // Timer Counter Register
    bool enabled;                 // Timer enabled
} Timer;

void timer_init(Timer *timer);
void timer_tick(Timer *timer);