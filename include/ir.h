#pragma once
#include "cpu.h"
#define IR_VBLANK_ADDR 0x40
#define IR_STAT_ADDR 0x48 // LCD STAT
#define IR_TIMER_ADDR 0x50
#define IR_SERIAL_ADDR 0x58
#define IR_JOYPAD_ADDR 0x60

// ir bit
#define IR_VBLANK 0
#define IR_STAT 1
#define IR_TIMER 2
#define IR_SERIAL 3
#define IR_JOYPAD 4

// mask
#define IR_VBLANK_MASK (1 << IR_VBLANK)
#define IR_STAT_MASK (1 << IR_STAT)
#define IR_TIMER_MASK (1 << IR_TIMER)
#define IR_SERIAL_MASK (1 << IR_SERIAL)
#define IR_JOYPAD_MASK (1 << IR_JOYPAD)

void ir_handel(CPU *cpu);