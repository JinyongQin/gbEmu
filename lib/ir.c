#include "ir.h"
#include "emu.h"

extern Emulator *emu;

static inline void push_pc(CPU *cpu)
{
    // push pc to stack
    memory_write(cpu->reg.sp - 1, cpu->reg.pc >> 8);
    memory_write(cpu->reg.sp - 2, cpu->reg.pc & 0xFF);
    cpu->reg.sp -= 2;
}

static inline uint16_t choose_ir(CPU *cpu, uint8_t ir)
{
    if (ir & IR_VBLANK_MASK)
    {
        // clear VBLANK flag
        emu->IF &= ~IR_VBLANK_MASK;
        // return VBLANK address
        return IR_VBLANK_ADDR;
    }
    else if (ir & IR_STAT_MASK)
    {
        emu->IF &= ~IR_STAT_MASK;
        return IR_STAT_ADDR;
    }
    else if (ir & IR_TIMER_MASK)
    {
        emu->IF &= ~IR_TIMER_MASK;
        return IR_TIMER_ADDR;
    }
    else if (ir & IR_SERIAL_MASK)
    {
        emu->IF &= ~IR_SERIAL_MASK;
        return IR_SERIAL_ADDR;
    }
    else if (ir & IR_JOYPAD_MASK)
    {
        emu->IF &= ~IR_JOYPAD_MASK;
        return IR_JOYPAD_ADDR;
    }
    else
    {
        // error
        printf("Error: unknown interrupt\n");
        exit(-3);
    }
}

// interrupt handler
void ir_handel(CPU *cpu)
{
    // 1. wait 2 cycles
    cpu->ime = false; // disable interrupts
    emu->tick_e(emu, 2);
    uint8_t ir = emu->IF & emu->IE;

    // 2. The current value of the PC register is pushed onto the stack, 2 more M-cycles.
    push_pc(cpu);
    emu->tick_e(emu, 2);
    // 3.The PC register is set to the address of the handler, 1 M-cycle.
    choose_ir(cpu, ir);
    emu->tick_e(emu, 1);
}