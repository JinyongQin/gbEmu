#pragma once

/*
gameboy cpu ：Sharp SM83

8-bit CPU
8-bit and 16-bit registers

Registers:
AF: 16-bit register: A (accumulator) and F (flags)
BC: 16-bit register: B and C (8-bit registers)
DE: 16-bit register: D and E (8-bit registers)
HL: 16-bit register: H and L (8-bit registers)
SP: 16-bit stack pointer, points to the top of the stack
PC: 16-bit program counter, points to the current instruction

Flag register:
bit 7: zero flag (Z)
bit 6: subtract flag (N)
bit 5: half carry flag (H)
bit 4: carry flag (C)
bit 3-0: not used
*/
#include "common.h"
#define reg8 uint8_t
#define reg16 uint16_t

#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4

#define set_flag(cpu, z, n, f, c) \
    cpu->reg.f = (z << FLAG_Z | n << FLAG_N | f << FLAG_H | c << FLAG_C) | 0xF0
#define get_flag(cpu, flag) ((cpu->reg.f >> flag) & 0x01)
#define set_flag_z(cpu, z) (cpu->reg.f = (cpu->reg.f & 0x7F) | (z << FLAG_Z))
#define set_flag_n(cpu, n) (cpu->reg.f = (cpu->reg.f & 0xBF) | (n << FLAG_N))
#define set_flag_h(cpu, h) (cpu->reg.f = (cpu->reg.f & 0xDF) | (h << FLAG_H))
#define set_flag_c(cpu, c) (cpu->reg.f = (cpu->reg.f & 0xEF) | (c << FLAG_C))

typedef struct
{
    union
    { // AF
        struct
        {
            reg8 f; // flags Low
            reg8 a; // accumulator High
        };
        reg16 af;
    };

    union
    {
        struct
        {
            reg8 c; // Low
            reg8 b; // High
        }; // BC 16-bit register
        reg16 bc;
    };
    union
    {
        struct
        {
            reg8 e; // Low
            reg8 d; // High
        }; // DE 16-bit register
        reg16 de;
    };
    union
    {
        struct
        {
            reg8 l; // Low
            reg8 h; // High
        }; // HL 16-bit register
        reg16 hl;
    };

    reg16 sp; // stack pointer

    reg16 pc; // program counter
} cpu_reg;

typedef struct CPU
{
    cpu_reg reg;
    bool ime;    // interrupt master enable
    bool halted; // cpu is halted
    int (*reset)(struct CPU *cpu);
    void (*step)(struct CPU *cpu);
} CPU;

int cpu_init(CPU *cpu);