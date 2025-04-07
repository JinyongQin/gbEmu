#include "timer.h"
#include "ir.h"
#include "emu.h"
extern Emulator *emu;
void timer_init(Timer *timer)
{
    timer->DIV = 0xAC00;
    timer->TMA = 0;
    timer->TAC = 0xF8;
    timer->TMC = 0;
    timer->enabled = false;
    // set div_update_bit : 00 = 9, 01 = 3, 10 = 5, 11 = 7
    timer->div_update_bit_mask = DIV_BIT_MASK(9);
}

void timer_tick(Timer *timer)
{
    // update DIV
    uint16_t last_div = timer->DIV;
    timer->DIV++;
    // update TMC: div bit[div_update_bit] change from 0 to 1
    if ((last_div & timer->div_update_bit_mask) == 0 && (timer->DIV & timer->div_update_bit_mask))
    {
        if (timer->TMC == 0xFF)
        { // at this time, tmc++, it will overflow
            timer->TMC = timer->TMA;
            // set IF bit
            emu->IF |= IR_TIMER_MASK;
        }
        else
        {
            timer->TMC++;
        }
    }
}
