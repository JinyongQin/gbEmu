#pragma once
#include "cpu.h"
#include "emu.h"
#include "mem.h"
#define OpCode uint8_t
typedef void (*opHandler)(CPU *); // it will handle the instruction and update the CPU state

extern Emulator *emu;
// read from memory, not register, spend 1 m_cycle
static inline uint8_t _memory_read(uint16_t addr)
{
    emu->tick_e(emu, 1);
    return memory_read(addr);
}
// write to memory, not register, spend 1 m_cycle
static inline void _memory_write(uint16_t addr, uint8_t val)
{
    emu->tick_e(emu, 1);
    memory_write(addr, val);
}

static void
nop(CPU *cpu)
{
    /*NOP
    Cycles: 1
    Bytes: 1
    Flags: None affected.
    */
}
/*---------------- Load Instructions-------------------------------------------------------------*/

/*Copy (aka Load) the value in r2 (8bit) into r1 (8bit).*/
#define LD_R8_R8(r1, r2)                 \
    static void ld_##r1##_##r2(CPU *cpu) \
    {                                    \
        cpu->reg.r1 = cpu->reg.r2;       \
    }
// generate ld r8 r8
LD_R8_R8(a, a);
LD_R8_R8(a, b);
LD_R8_R8(a, c);
LD_R8_R8(a, d);
LD_R8_R8(a, e);
LD_R8_R8(a, h);
LD_R8_R8(a, l);
LD_R8_R8(b, a);
LD_R8_R8(b, b);
LD_R8_R8(b, c);
LD_R8_R8(b, d);
LD_R8_R8(b, e);
LD_R8_R8(b, h);
LD_R8_R8(b, l);
LD_R8_R8(c, a);
LD_R8_R8(c, b);
LD_R8_R8(c, c);
LD_R8_R8(c, d);
LD_R8_R8(c, e);
LD_R8_R8(c, h);
LD_R8_R8(c, l);
LD_R8_R8(d, a);
LD_R8_R8(d, b);
LD_R8_R8(d, c);
LD_R8_R8(d, d);
LD_R8_R8(d, e);
LD_R8_R8(d, h);
LD_R8_R8(d, l);
LD_R8_R8(e, a);
LD_R8_R8(e, b);
LD_R8_R8(e, c);
LD_R8_R8(e, d);
LD_R8_R8(e, e);
LD_R8_R8(e, h);
LD_R8_R8(e, l);
LD_R8_R8(h, a);
LD_R8_R8(h, b);
LD_R8_R8(h, c);
LD_R8_R8(h, d);
LD_R8_R8(h, e);
LD_R8_R8(h, h);
LD_R8_R8(h, l);
LD_R8_R8(l, a);
LD_R8_R8(l, b);
LD_R8_R8(l, c);
LD_R8_R8(l, d);
LD_R8_R8(l, e);
LD_R8_R8(l, h);
LD_R8_R8(l, l);

/*read the value in the memory address pointed to by the r2 (16bit) into r1 (8bit).*/
#define LD_R8_MR16(r1, r2)                       \
    static void ld_##r1##_m##r2(CPU *cpu)        \
    {                                            \
        cpu->reg.r1 = _memory_read(cpu->reg.r2); \
    }
// generate ld r8 mr16
LD_R8_MR16(a, bc);
LD_R8_MR16(a, de);
LD_R8_MR16(a, hl);
LD_R8_MR16(b, hl);
LD_R8_MR16(c, hl);
LD_R8_MR16(d, hl);
LD_R8_MR16(e, hl);
LD_R8_MR16(h, hl);
LD_R8_MR16(l, hl);

/*write the value in r1 (8bit) into the memory address pointed to by the r2 (16bit).*/
#define LD_MR16_R8(r1, r2)                       \
    static void ld_m##r1##_##r2(CPU *cpu)        \
    {                                            \
        _memory_write(cpu->reg.r1, cpu->reg.r2); \
    }
// generate ld mr16 r8
LD_MR16_R8(bc, a);
LD_MR16_R8(de, a);
LD_MR16_R8(hl, a);
LD_MR16_R8(hl, b);
LD_MR16_R8(hl, c);
LD_MR16_R8(hl, d);
LD_MR16_R8(hl, e);
LD_MR16_R8(hl, h);
LD_MR16_R8(hl, l);

/*Loads 8-bit immediate data to r*/
#define LD_R8_D8(r)                             \
    static void ld_##r##_d8(CPU *cpu)           \
    {                                           \
        cpu->reg.r = _memory_read(cpu->reg.pc); \
        cpu->reg.pc++;                          \
    }
// generate ld r8 d8
LD_R8_D8(a);
LD_R8_D8(b);
LD_R8_D8(c);
LD_R8_D8(d);
LD_R8_D8(e);
LD_R8_D8(h);
LD_R8_D8(l);
// LD (HL+), A
static void ld_mhl_a_inc(CPU *cpu)
{
    _memory_write(cpu->reg.hl, cpu->reg.a);
    cpu->reg.hl++;
}
// LD (HL-), A
static void ld_mhl_a_dec(CPU *cpu)
{
    _memory_write(cpu->reg.hl, cpu->reg.a);
    cpu->reg.hl--;
}
// LD A, (HL+)
static void ld_a_mhl_inc(CPU *cpu)
{
    cpu->reg.a = _memory_read(cpu->reg.hl);
    cpu->reg.hl++;
}
// LD A, (HL-)
static void ld_a_mhl_dec(CPU *cpu)
{
    cpu->reg.a = _memory_read(cpu->reg.hl);
    cpu->reg.hl--;
}
// LDH [C], A ：write A to address 0xFF00 + C
static void ldh_mc_a(CPU *cpu)
{
    _memory_write(0xFF00 + cpu->reg.c, cpu->reg.a);
}
// LDH A, [C] ：read A from address 0xFF00 + C
static void ldh_a_mc(CPU *cpu)
{
    cpu->reg.a = _memory_read(0xFF00 + cpu->reg.c);
}

// LD [HL], d8
static void ld_mhl_d8(CPU *cpu)
{

    uint8_t d8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    _memory_write(cpu->reg.hl, d8);
}

// LDH [d8], A : write A to address 0xFF00 + d8
static void ldh_md8_a(CPU *cpu)
{

    uint8_t d8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    _memory_write(0xFF00 + d8, cpu->reg.a);
}
// LDH A, [d8] : read A from address 0xFF00 + d8
static void ldh_a_md8(CPU *cpu)
{

    uint8_t d8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    cpu->reg.a = _memory_read(0xFF00 | d8);
}

// LD r16 d16
#define LD_R16_D16(r)                                   \
    static void ld_##r##_d16(CPU *cpu)                  \
    {                                                   \
        cpu->reg.r = _memory_read(cpu->reg.pc);         \
        cpu->reg.pc++;                                  \
        cpu->reg.r |= (_memory_read(cpu->reg.pc) << 8); \
        cpu->reg.pc++;                                  \
    }
// generate ld r16 d16
LD_R16_D16(bc);
LD_R16_D16(de);
LD_R16_D16(hl);
LD_R16_D16(sp);
// LD [d16], SP
static void ld_md16_sp(CPU *cpu)
{
    // low byte
    uint8_t d = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    _memory_write(cpu->reg.sp, d);
    // high byte
    d = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    _memory_write(cpu->reg.sp + 1, d);
}
// LD [d16], A
static void ld_md16_a(CPU *cpu)
{
    uint16_t d16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    d16 |= (_memory_read(cpu->reg.pc) << 8);
    cpu->reg.pc++;
    _memory_write(d16, cpu->reg.a);
}
// LD A, [d16]
static void ld_a_md16(CPU *cpu)
{
    uint16_t d16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    d16 |= (_memory_read(cpu->reg.pc) << 8);
    cpu->reg.pc++;
    cpu->reg.a = _memory_read(d16);
}

// LD SP, HL
static void ld_sp_hl(CPU *cpu)
{
    cpu->reg.sp = cpu->reg.hl;
    emu->tick_e(emu, 1);
}
// LD HL,SP+e8
static void ld_hl_sp_e8(CPU *cpu)
{

    int8_t e8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    uint16_t sp = cpu->reg.sp;
    uint16_t hl = sp + e8;
    // set flag z:0 n:0
    // h:1 Set if overflow from bit 3.
    // c:1 Set if overflow from bit 7.
    uint16_t check = e8 ^ sp ^ hl;
    set_flag_h(cpu, (check & 0x10));
    set_flag_c(cpu, (check & 0x100));
    set_flag_z(cpu, 0);
    set_flag_n(cpu, 0);
    cpu->reg.hl = hl;
    emu->tick_e(emu, 1);
}

/*-------------------------------Compare instruction-------------------------------------*/
// cp a r8
#define CP_A_R8(r)                                                    \
    static void cp_a_##r(CPU *cpu)                                    \
    {                                                                 \
        uint8_t res = cpu->reg.a - cpu->reg.r;                        \
        set_flag_z(cpu, (res == 0));                                  \
        set_flag_n(cpu, 1);                                           \
        set_flag_h(cpu, ((cpu->reg.a & 0x0F) < (cpu->reg.r & 0x0F))); \
        set_flag_c(cpu, (cpu->reg.a < cpu->reg.r));                   \
    }
// generate cp a r8
CP_A_R8(a);
CP_A_R8(b);
CP_A_R8(c);
CP_A_R8(d);
CP_A_R8(e);
CP_A_R8(h);
CP_A_R8(l);
// CP A, [HL]
static void cp_a_mhl(CPU *cpu)
{

    uint8_t d8 = _memory_read(cpu->reg.hl);
    uint8_t res = cpu->reg.a - d8;
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 1);
    set_flag_h(cpu, ((cpu->reg.a & 0x0F) < (d8 & 0x0F)));
    set_flag_c(cpu, (cpu->reg.a < d8));
}
// CP A, d8
static void cp_a_d8(CPU *cpu)
{
    uint8_t d8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    uint8_t res = cpu->reg.a - d8;
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 1);
    set_flag_h(cpu, ((cpu->reg.a & 0x0F) < (d8 & 0x0F)));
    set_flag_c(cpu, (cpu->reg.a < d8));
}

/*--------------------------------Jump Instruction-------------------------------------*/
// JP a16
static void jp_a16(CPU *cpu)
{
    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    cpu->reg.pc = a16;
    // pause 1 m_ticks
    emu->tick_e(emu, 1);
}

// JP NZ, a16
static void jp_nz_a16(CPU *cpu)
{

    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    if (!get_flag(cpu, FLAG_Z))
    {
        cpu->reg.pc = a16;
        emu->tick_e(emu, 1);
    }
    else
    {
        cpu->reg.pc++;
    }
}
// JP Z, a16
static void jp_z_a16(CPU *cpu)
{

    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    if (get_flag(cpu, FLAG_Z))
    {
        cpu->reg.pc = a16;
        emu->tick_e(emu, 1);
    }
    else
    {

        cpu->reg.pc++;
    }
}
// JP NC, a16
static void jp_nc_a16(CPU *cpu)
{

    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    if (!get_flag(cpu, FLAG_C))
    {
        cpu->reg.pc = a16;
        emu->tick_e(emu, 1);
    }
    else
    {

        cpu->reg.pc++;
    }
}
// JP C, a16
static void jp_c_a16(CPU *cpu)
{

    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    if (get_flag(cpu, FLAG_C))
    {
        cpu->reg.pc = a16;
        emu->tick_e(emu, 1);
    }
    else
    {

        cpu->reg.pc++;
    }
}
// JP [HL]
static void jp_mhl(CPU *cpu)
{
    cpu->reg.pc = cpu->reg.hl;
}

// JR e8
static void jr_e8(CPU *cpu)
{
    int8_t e8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    cpu->reg.pc += e8;
    emu->tick_e(emu, 1);
}

// JR NZ, e8
static void jr_nz_e8(CPU *cpu)
{
    int8_t e8 = (int8_t)_memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    if (!get_flag(cpu, FLAG_Z))
    {
        cpu->reg.pc += (int16_t)e8;
        emu->tick_e(emu, 1);
    }
}
// JR Z, e8
static void jr_z_e8(CPU *cpu)
{
    int8_t e8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    if (get_flag(cpu, FLAG_Z))
    {
        cpu->reg.pc += e8;
        emu->tick_e(emu, 1);
    }
}
// JR NC, e8
static void jr_nc_e8(CPU *cpu)
{
    int8_t e8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    if (!get_flag(cpu, FLAG_C))
    {
        cpu->reg.pc += e8;
        emu->tick_e(emu, 1);
    }
}
// JR C, e8
static void jr_c_e8(CPU *cpu)
{
    int8_t e8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    if (get_flag(cpu, FLAG_C))
    {
        cpu->reg.pc += e8;
        emu->tick_e(emu, 1);
    }
}
// CALL a16
static void call_a16(CPU *cpu)
{

    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    cpu->reg.pc++;
    // sp low
    cpu->reg.sp -= 2;
    _memory_write(cpu->reg.sp, cpu->reg.pc & 0xFF);
    _memory_write(cpu->reg.sp + 1, (cpu->reg.pc >> 8));
    cpu->reg.pc = a16;
    emu->tick_e(emu, 1);
}

// CALL NZ, a16
static void call_nz_a16(CPU *cpu)
{
    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    cpu->reg.pc++;
    if (!get_flag(cpu, FLAG_Z))
    {
        cpu->reg.sp -= 2;
        _memory_write(cpu->reg.sp, cpu->reg.pc & 0xFF);
        _memory_write(cpu->reg.sp + 1, (cpu->reg.pc >> 8));
        cpu->reg.pc = a16;
        emu->tick_e(emu, 1);
    }
}
// CALL Z, a16
static void call_z_a16(CPU *cpu)
{
    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    cpu->reg.pc++;
    if (get_flag(cpu, FLAG_Z))
    {
        cpu->reg.sp -= 2;
        _memory_write(cpu->reg.sp, cpu->reg.pc & 0xFF);
        _memory_write(cpu->reg.sp + 1, (cpu->reg.pc >> 8));
        cpu->reg.pc = a16;
        emu->tick_e(emu, 1);
    }
}

// CALL NC, a16
static void call_nc_a16(CPU *cpu)
{
    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    cpu->reg.pc++;
    if (!get_flag(cpu, FLAG_C))
    {
        cpu->reg.sp -= 2;
        _memory_write(cpu->reg.sp, cpu->reg.pc & 0xFF);
        _memory_write(cpu->reg.sp + 1, (cpu->reg.pc >> 8));
        cpu->reg.pc = a16;
        emu->tick_e(emu, 1);
    }
}
// CALL C, a16
static void call_c_a16(CPU *cpu)
{

    uint16_t a16 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    a16 |= (_memory_read(cpu->reg.pc) << 8);
    cpu->reg.pc++;
    if (get_flag(cpu, FLAG_C))
    {
        cpu->reg.sp -= 2;
        _memory_write(cpu->reg.sp, cpu->reg.pc & 0xFF);
        _memory_write(cpu->reg.sp + 1, (cpu->reg.pc >> 8));
        cpu->reg.pc = a16;
        emu->tick_e(emu, 1);
    }
}

// RET
static void ret(CPU *cpu)
{
    cpu->reg.pc = _memory_read(cpu->reg.sp);
    cpu->reg.pc |= (_memory_read(cpu->reg.sp + 1) << 8);
    cpu->reg.sp += 2;
    emu->tick_e(emu, 1);
}
// RET NZ
static void ret_nz(CPU *cpu)
{
    emu->tick_e(emu, 1);
    if (!get_flag(cpu, FLAG_Z))
    {
        ret(cpu);
    }
}
// RET Z
static void ret_z(CPU *cpu)
{
    emu->tick_e(emu, 1);
    if (get_flag(cpu, FLAG_Z))
    {
        ret(cpu);
    }
}

// RET NC
static void ret_nc(CPU *cpu)
{
    emu->tick_e(emu, 1);
    if (!get_flag(cpu, FLAG_C))
    {
        ret(cpu);
    }
}
// RET C
static void ret_c(CPU *cpu)
{
    emu->tick_e(emu, 1);
    if (get_flag(cpu, FLAG_C))
    {
        ret(cpu);
    }
}

// RETI
static void reti(CPU *cpu)
{
    ret(cpu);
    cpu->ime = true; // enable interrupts
}

// RST n , n=0, 8, 10, 18, 20, 28, 30 , 38 (hex)
#define RST_N(n)                                            \
    static void rst_##n(CPU *cpu)                           \
    {                                                       \
        cpu->reg.sp -= 2;                                   \
        _memory_write(cpu->reg.sp, cpu->reg.pc & 0xFF);     \
        _memory_write(cpu->reg.sp + 1, (cpu->reg.pc >> 8)); \
        cpu->reg.pc = n;                                    \
        emu->tick_e(emu, 1);                                \
    }
RST_N(0x0000)
RST_N(0x0008)
RST_N(0x0010)
RST_N(0x0018)
RST_N(0x0020)
RST_N(0x0028)
RST_N(0x0030)
RST_N(0x0038)
/*--------------------------------stack opration----------------------------------*/
// PUSH r16
#define PUSH_R16(r)                                        \
    static void push_##r(CPU *cpu)                         \
    {                                                      \
        cpu->reg.sp -= 2;                                  \
        _memory_write(cpu->reg.sp, cpu->reg.r & 0xFF);     \
        _memory_write(cpu->reg.sp + 1, (cpu->reg.r >> 8)); \
        emu->tick_e(emu, 1);                               \
    }
// generate push r16
PUSH_R16(bc);
PUSH_R16(de);
PUSH_R16(hl);
PUSH_R16(af);
// POP r16
#define POP_R16(r)                                          \
    static void pop_##r(CPU *cpu)                           \
    {                                                       \
                                                            \
        cpu->reg.r = _memory_read(cpu->reg.sp);             \
        cpu->reg.r |= (_memory_read(cpu->reg.sp + 1) << 8); \
        cpu->reg.sp += 2;                                   \
    }
POP_R16(bc);
POP_R16(de);
POP_R16(hl);
POP_R16(af);

/*-------------------------------Arithmetic instruction-------------------------------------*/

// INC r8
#define INC_R8(r)                                    \
    static void inc_##r(CPU *cpu)                    \
    {                                                \
        cpu->reg.r++;                                \
        set_flag_z(cpu, (cpu->reg.r == 0));          \
        set_flag_n(cpu, 0);                          \
        set_flag_h(cpu, ((cpu->reg.r & 0x0F) == 0)); \
    }
INC_R8(a);
INC_R8(b);
INC_R8(c);
INC_R8(d);
INC_R8(e);
INC_R8(h);
INC_R8(l);
// INC [HL]
static void inc_mhl(CPU *cpu)
{
    uint8_t val = _memory_read(cpu->reg.hl);
    val++;
    _memory_write(cpu->reg.hl, val);
    set_flag_z(cpu, (val == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, ((val & 0x0F) == 0));
}

// INC r16
#define INC_R16(r)                \
    static void inc_##r(CPU *cpu) \
    {                             \
        cpu->reg.r++;             \
        emu->tick_e(emu, 1);      \
    }
INC_R16(bc);
INC_R16(de);
INC_R16(hl);
INC_R16(sp);
// DEC r8
#define DEC_R8(r)                                    \
    static void dec_##r(CPU *cpu)                    \
    {                                                \
        cpu->reg.r--;                                \
        set_flag_z(cpu, (cpu->reg.r == 0));          \
        set_flag_n(cpu, 1);                          \
        set_flag_h(cpu, ((cpu->reg.r & 0x0F) == 0)); \
    }
DEC_R8(a);
DEC_R8(b);
DEC_R8(c);
DEC_R8(d);
DEC_R8(e);
DEC_R8(h);
DEC_R8(l);
// DEC [HL]
static void dec_mhl(CPU *cpu)
{
    uint8_t val = _memory_read(cpu->reg.hl);
    val--;
    _memory_write(cpu->reg.hl, val);
    set_flag_z(cpu, (val == 0));
    set_flag_n(cpu, 1);
    set_flag_h(cpu, ((val & 0x0F) == 0));
}

// DEC r16
#define DEC_R16(r)                \
    static void dec_##r(CPU *cpu) \
    {                             \
        cpu->reg.r--;             \
        emu->tick_e(emu, 1);      \
    }
DEC_R16(bc);
DEC_R16(de);
DEC_R16(hl);
DEC_R16(sp);
static inline void add8(CPU *cpu, uint8_t a, uint8_t b)
{
    uint16_t res = a + b;
    set_flag_z(cpu, (res & 0xFF) == 0);
    set_flag_n(cpu, 0);
    set_flag_h(cpu, ((a & 0x0F) + (b & 0x0F)) > 0x0F);
    set_flag_c(cpu, (res > 0xFF));
    cpu->reg.a = res & 0xFF;
}

// ADD A, r8
#define ADD_A_R8(r)                        \
    static void add_a_##r(CPU *cpu)        \
    {                                      \
        add8(cpu, cpu->reg.a, cpu->reg.r); \
    }
ADD_A_R8(a);
ADD_A_R8(b);
ADD_A_R8(c);
ADD_A_R8(d);
ADD_A_R8(e);
ADD_A_R8(h);
ADD_A_R8(l);
// ADD A, d8
static void add_a_d8(CPU *cpu)
{
    add8(cpu, cpu->reg.a, _memory_read(cpu->reg.pc));
    cpu->reg.pc++;
}

// ADD A, [HL]
static void add_a_mhl(CPU *cpu)
{
    add8(cpu, cpu->reg.a, _memory_read(cpu->reg.hl));
}

// 16bit addition
static inline void add16(CPU *cpu, uint16_t a, uint16_t b)
{
    uint32_t res = a + b;
    set_flag_z(cpu, (res & 0xFFFF) == 0);
    set_flag_n(cpu, 0);
    set_flag_h(cpu, ((a & 0x0FFF) + (b & 0x0FFF)) > 0x0FFF);
    set_flag_c(cpu, (res > 0xFFFF));
}

// ADD HL, r16
#define ADD_HL_R16(r)                        \
    static void add_hl_##r(CPU *cpu)         \
    {                                        \
        add16(cpu, cpu->reg.hl, cpu->reg.r); \
        emu->tick_e(emu, 1);                 \
    }
ADD_HL_R16(bc);
ADD_HL_R16(de);
ADD_HL_R16(hl);
ADD_HL_R16(sp);
// ADD SP, e8
static void add_sp_e8(CPU *cpu)
{
    int8_t e8 = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    uint16_t res = cpu->reg.sp + e8;
    set_flag_z(cpu, (res & 0xFFFF) == 0);
    set_flag_n(cpu, 0);
    set_flag_h(cpu, ((cpu->reg.sp & 0x0FFF) + (e8 & 0x0FFF)) > 0x0FFF);
    set_flag_c(cpu, (res > 0xFFFF));
    cpu->reg.sp = res;
    emu->tick_e(emu, 2);
}
// ADC8
static inline void adc8(CPU *cpu, uint8_t a, uint8_t b)
{
    uint16_t res = a + b + get_flag(cpu, FLAG_C);
    set_flag_z(cpu, (res & 0xFF) == 0);
    set_flag_n(cpu, 0);
    set_flag_h(cpu, ((a & 0x0F) + (b & 0x0F) + get_flag(cpu, FLAG_C)) > 0x0F);
    set_flag_c(cpu, (res > 0xFF));
    cpu->reg.a = res & 0xFF;
}

// ADC A, r8
#define ADC_A_R8(r)                        \
    static void adc_a_##r(CPU *cpu)        \
    {                                      \
        adc8(cpu, cpu->reg.a, cpu->reg.r); \
    }
ADC_A_R8(a);
ADC_A_R8(b);
ADC_A_R8(c);
ADC_A_R8(d);
ADC_A_R8(e);
ADC_A_R8(h);
ADC_A_R8(l);
// ADC A, d8
static void adc_a_d8(CPU *cpu)
{
    adc8(cpu, cpu->reg.a, _memory_read(cpu->reg.pc));
    cpu->reg.pc++;
}
// ADC A, [HL]
static void adc_a_mhl(CPU *cpu)
{
    adc8(cpu, cpu->reg.a, _memory_read(cpu->reg.hl));
}

// SUB8
static inline void sub8(CPU *cpu, uint8_t a, uint8_t b)
{
    uint16_t res = a - b;
    set_flag_z(cpu, (res & 0xFF) == 0);
    set_flag_n(cpu, 1);
    set_flag_h(cpu, ((a & 0x0F) - (b & 0x0F)) < 0);
    set_flag_c(cpu, (res > 0xFF));
    cpu->reg.a = res & 0xFF;
}

// SUB A, r8
#define SUB_A_R8(r)                        \
    static void sub_a_##r(CPU *cpu)        \
    {                                      \
        sub8(cpu, cpu->reg.a, cpu->reg.r); \
    }
SUB_A_R8(a);
SUB_A_R8(b);
SUB_A_R8(c);
SUB_A_R8(d);
SUB_A_R8(e);
SUB_A_R8(h);
SUB_A_R8(l);
// SUB A, d8
static void sub_a_d8(CPU *cpu)
{
    sub8(cpu, cpu->reg.a, _memory_read(cpu->reg.pc));
    cpu->reg.pc++;
}

// SUB A, [HL]
static void sub_a_mhl(CPU *cpu)
{
    sub8(cpu, cpu->reg.a, _memory_read(cpu->reg.hl));
}
// SBC8
static inline void sbc8(CPU *cpu, uint8_t a, uint8_t b)
{
    uint16_t res = a - b - get_flag(cpu, FLAG_C);
    set_flag_z(cpu, (res & 0xFF) == 0);
    set_flag_n(cpu, 1);
    set_flag_h(cpu, ((a & 0x0F) - (b & 0x0F) - get_flag(cpu, FLAG_C)) < 0);
    set_flag_c(cpu, (res > 0xFF));
    cpu->reg.a = res & 0xFF;
}
// SBC A, r8
#define SBC_A_R8(r)                        \
    static void sbc_a_##r(CPU *cpu)        \
    {                                      \
        sbc8(cpu, cpu->reg.a, cpu->reg.r); \
    }
SBC_A_R8(a);
SBC_A_R8(b);
SBC_A_R8(c);
SBC_A_R8(d);
SBC_A_R8(e);
SBC_A_R8(h);
SBC_A_R8(l);
// SBC A, d8
static void sbc_a_d8(CPU *cpu)
{
    sbc8(cpu, cpu->reg.a, _memory_read(cpu->reg.pc));
    cpu->reg.pc++;
}
// SBC A, [HL]
static void sbc_a_mhl(CPU *cpu)
{
    sbc8(cpu, cpu->reg.a, _memory_read(cpu->reg.hl));
}
// AND8
static inline void and8(CPU *cpu, uint8_t a, uint8_t b)
{
    uint8_t res = a & b;
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 1);
    set_flag_c(cpu, 0);
    cpu->reg.a = res;
}
// AND A, r8
#define AND_A_R8(r)                        \
    static void and_a_##r(CPU *cpu)        \
    {                                      \
        and8(cpu, cpu->reg.a, cpu->reg.r); \
    }
AND_A_R8(a);
AND_A_R8(b);
AND_A_R8(c);
AND_A_R8(d);
AND_A_R8(e);
AND_A_R8(h);
AND_A_R8(l);
// AND A, d8
static void and_a_d8(CPU *cpu)
{
    and8(cpu, cpu->reg.a, _memory_read(cpu->reg.pc));
    cpu->reg.pc++;
}
// AND A, [HL]
static void and_a_mhl(CPU *cpu)
{
    and8(cpu, cpu->reg.a, _memory_read(cpu->reg.hl));
}
// xor8
static inline void xor8(CPU *cpu, uint8_t a, uint8_t b)
{
    uint8_t res = a ^ b;
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, 0);
    cpu->reg.a = res;
}

// XOR A, r8
#define XOR_A_R8(r)                        \
    static void xor_a_##r(CPU *cpu)        \
    {                                      \
        xor8(cpu, cpu->reg.a, cpu->reg.r); \
    }
XOR_A_R8(a);
XOR_A_R8(b);
XOR_A_R8(c);
XOR_A_R8(d);
XOR_A_R8(e);
XOR_A_R8(h);
XOR_A_R8(l);
// XOR A, d8
static void xor_a_d8(CPU *cpu)
{
    xor8(cpu, cpu->reg.a, _memory_read(cpu->reg.pc));
    cpu->reg.pc++;
}

// XOR A, [HL]
static void xor_a_mhl(CPU *cpu)
{
    xor8(cpu, cpu->reg.a, _memory_read(cpu->reg.hl));
}
// OR8
static inline void or8(CPU *cpu, uint8_t a, uint8_t b)
{
    uint8_t res = a | b;
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, 0);
    cpu->reg.a = res;
}

// OR A, r8
#define OR_A_R8(r)                        \
    static void or_a_##r(CPU *cpu)        \
    {                                     \
        or8(cpu, cpu->reg.a, cpu->reg.r); \
    }
OR_A_R8(a);
OR_A_R8(b);
OR_A_R8(c);
OR_A_R8(d);
OR_A_R8(e);
OR_A_R8(h);
OR_A_R8(l);
// OR A, d8
static void or_a_d8(CPU *cpu)
{
    or8(cpu, cpu->reg.a, _memory_read(cpu->reg.pc));
    cpu->reg.pc++;
}
// OR A, [HL]
static void or_a_mhl(CPU *cpu)
{
    or8(cpu, cpu->reg.a, _memory_read(cpu->reg.hl));
}

// CPL
static void cpl(CPU *cpu)
{
    cpu->reg.a = ~cpu->reg.a;
    set_flag_n(cpu, 1);
    set_flag_h(cpu, 1);
}

// CCF
static void ccf(CPU *cpu)
{
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, !get_flag(cpu, FLAG_C));
}

// SCF
static void scf(CPU *cpu)
{
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, 1);
}

// DAA
static void daa(CPU *cpu)
{
    uint8_t a = cpu->reg.a;
    if (get_flag(cpu, FLAG_N))
    {
        if (get_flag(cpu, FLAG_C))
            a -= 0x60;
        if (get_flag(cpu, FLAG_H))
            a -= 0x06;
    }
    else
    {
        if (get_flag(cpu, FLAG_C) || (a > 0x99))
            a += 0x60;
        if (get_flag(cpu, FLAG_H) || ((a & 0x0F) > 0x09))
            a += 0x06;
    }
    cpu->reg.a = a;
    set_flag_z(cpu, (cpu->reg.a == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
}
// 0x07 RLCA 将Z标志位固定设置为0，而不是根据结果来设置，并且只花费一个机器周期
static void rlca(CPU *cpu)
{
    uint8_t res = (cpu->reg.a << 1) | (cpu->reg.a >> 7);
    set_flag_z(cpu, 0);
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (cpu->reg.a & 0x80));
    cpu->reg.a = res;
}

// 0x0F RRCA
static void rrca(CPU *cpu)
{
    uint8_t res = (cpu->reg.a >> 1) | (cpu->reg.a << 7);
    set_flag_z(cpu, 0);
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (cpu->reg.a & 0x01));
    cpu->reg.a = res;
}
// 0x17 RLA
static void rla(CPU *cpu)
{
    uint8_t res = (cpu->reg.a << 1) | get_flag(cpu, FLAG_C);
    set_flag_z(cpu, 0);
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (cpu->reg.a & 0x80));
    cpu->reg.a = res;
}
// 0x1F RRA
static void rra(CPU *cpu)
{
    uint8_t res = (cpu->reg.a >> 1) | (get_flag(cpu, FLAG_C) << 7);
    set_flag_z(cpu, 0);
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (cpu->reg.a & 0x01));
    cpu->reg.a = res;
}
/*################################# CPU control ######################################*/
// 0x76 HALT
static void halt(CPU *cpu)
{
    cpu->halted = 1;
    // CPU处于暂停状态，直到发生中断
}

// 0x10 STOP  停止emu
static void stop(CPU *cpu)
{
    emu->running = 0;
}
/*############################### interrupt #########################################*/

// 0xD9 EI
static void ei(CPU *cpu)
{
    cpu->ime = 1;
}
// 0xFB DI
static void di(CPU *cpu)
{
    cpu->ime = 0;
}

/*####################################################################*/
/*                       0xCB code                                    */
/*####################################################################*/
/*
第二个字节的值的低三位表示操作的目标，这些目标与对应的值如下所示：
操作码	目标
000b	B寄存器
001b	C寄存器
010b	D寄存器
011b	E寄存器
100b	H寄存器
101b	L寄存器
110b	HL寄存器所存储的地址
111b	A寄存器
*/

// RLC r8
#define RLC_R8(r)                                            \
    static void rlc_##r(CPU *cpu)                            \
    {                                                        \
        uint8_t res = (cpu->reg.r << 1) | (cpu->reg.r >> 7); \
        set_flag_z(cpu, (res == 0));                         \
        set_flag_n(cpu, 0);                                  \
        set_flag_h(cpu, 0);                                  \
        set_flag_c(cpu, (cpu->reg.r & 0x80));                \
        cpu->reg.r = res;                                    \
    }
RLC_R8(a);
RLC_R8(b);
RLC_R8(c);
RLC_R8(d);
RLC_R8(e);
RLC_R8(h);
RLC_R8(l);
// RLC [HL]
static void rlc_mhl(CPU *cpu)
{

    uint8_t val = _memory_read(cpu->reg.hl);
    uint8_t res = (val << 1) | (val >> 7);
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (val & 0x80));
    _memory_write(cpu->reg.hl, res);
}
// RRC r8
#define RRC_R8(r)                                            \
    static void rrc_##r(CPU *cpu)                            \
    {                                                        \
        uint8_t res = (cpu->reg.r >> 1) | (cpu->reg.r << 7); \
        set_flag_z(cpu, (res == 0));                         \
        set_flag_n(cpu, 0);                                  \
        set_flag_h(cpu, 0);                                  \
        set_flag_c(cpu, (cpu->reg.r & 0x01));                \
        cpu->reg.r = res;                                    \
    }
RRC_R8(a);
RRC_R8(b);
RRC_R8(c);
RRC_R8(d);
RRC_R8(e);
RRC_R8(h);
RRC_R8(l);
// RRC [HL]
static void rrc_mhl(CPU *cpu)
{
    uint8_t val = _memory_read(cpu->reg.hl);
    uint8_t res = (val >> 1) | (val << 7);
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (val & 0x01));
    _memory_write(cpu->reg.hl, res);
}
// RL r8
#define RL_R8(r)                                                 \
    static void rl_##r(CPU *cpu)                                 \
    {                                                            \
                                                                 \
        uint8_t res = (cpu->reg.r << 1) | get_flag(cpu, FLAG_C); \
        set_flag_z(cpu, (res == 0));                             \
        set_flag_n(cpu, 0);                                      \
        set_flag_h(cpu, 0);                                      \
        set_flag_c(cpu, (cpu->reg.r & 0x80));                    \
        cpu->reg.r = res;                                        \
                                                                 \
        emu->tick_e(emu, 2);                                     \
    }
RL_R8(a);
RL_R8(b);
RL_R8(c);
RL_R8(d);
RL_R8(e);
RL_R8(h);
RL_R8(l);
// RL [HL]
static void rl_mhl(CPU *cpu)
{
    uint8_t val = _memory_read(cpu->reg.hl);
    uint8_t res = (val << 1) | get_flag(cpu, FLAG_C);
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (val & 0x80));
    _memory_write(cpu->reg.hl, res);
}
// RR r8
#define RR_R8(r)                                                        \
    static void rr_##r(CPU *cpu)                                        \
    {                                                                   \
        uint8_t res = (cpu->reg.r >> 1) | (get_flag(cpu, FLAG_C) << 7); \
        set_flag_z(cpu, (res == 0));                                    \
        set_flag_n(cpu, 0);                                             \
        set_flag_h(cpu, 0);                                             \
        set_flag_c(cpu, (cpu->reg.r & 0x01));                           \
        cpu->reg.r = res;                                               \
    }
RR_R8(a);
RR_R8(b);
RR_R8(c);
RR_R8(d);
RR_R8(e);
RR_R8(h);
RR_R8(l);
// RR [HL]
static void rr_mhl(CPU *cpu)
{
    uint8_t val = _memory_read(cpu->reg.hl);
    uint8_t res = (val >> 1) | (get_flag(cpu, FLAG_C) << 7);
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (val & 0x01));
    _memory_write(cpu->reg.hl, res);
}
// SLA r8
#define SLA_R8(r)                             \
    static void sla_##r(CPU *cpu)             \
    {                                         \
        set_flag_z(cpu, (cpu->reg.r == 0));   \
        set_flag_n(cpu, 0);                   \
        set_flag_h(cpu, 0);                   \
        set_flag_c(cpu, (cpu->reg.r & 0x80)); \
        cpu->reg.r <<= 1;                     \
    }
SLA_R8(a);
SLA_R8(b);
SLA_R8(c);
SLA_R8(d);
SLA_R8(e);
SLA_R8(h);
SLA_R8(l);
// SLA [HL]
static void sla_mhl(CPU *cpu)
{
    uint8_t val = _memory_read(cpu->reg.hl);
    set_flag_z(cpu, (val == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (val & 0x80));
    val <<= 1;
    _memory_write(cpu->reg.hl, val);
}
// SRA r8
#define SRA_R8(r)                                             \
    static void sra_##r(CPU *cpu)                             \
    {                                                         \
        set_flag_z(cpu, (cpu->reg.r == 0));                   \
        set_flag_n(cpu, 0);                                   \
        set_flag_h(cpu, 0);                                   \
        set_flag_c(cpu, (cpu->reg.r & 0x01));                 \
        cpu->reg.r = (cpu->reg.r >> 1) | (cpu->reg.r & 0x80); \
    }
SRA_R8(a);
SRA_R8(b);
SRA_R8(c);
SRA_R8(d);
SRA_R8(e);
SRA_R8(h);
SRA_R8(l);
// SRA [HL]
static void sra_mhl(CPU *cpu)
{
    uint8_t val = _memory_read(cpu->reg.hl);
    set_flag_z(cpu, (val == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (val & 0x01));
    val = (val >> 1) | (val & 0x80);
    _memory_write(cpu->reg.hl, val);
}

// SWAP r8
#define SWAP_R8(r)                                                             \
    static void swap_##r(CPU *cpu)                                             \
    {                                                                          \
        uint8_t res = ((cpu->reg.r & 0x0F) << 4) | ((cpu->reg.r & 0xF0) >> 4); \
        set_flag_z(cpu, (res == 0));                                           \
        set_flag_n(cpu, 0);                                                    \
        set_flag_h(cpu, 0);                                                    \
        set_flag_c(cpu, 0);                                                    \
        cpu->reg.r = res;                                                      \
    }
SWAP_R8(a);
SWAP_R8(b);
SWAP_R8(c);
SWAP_R8(d);
SWAP_R8(e);
SWAP_R8(h);
SWAP_R8(l);
// SWAP [HL]
static void swap_mhl(CPU *cpu)
{
    uint8_t val = _memory_read(cpu->reg.hl);
    uint8_t res = ((val & 0x0F) << 4) | ((val & 0xF0) >> 4);
    set_flag_z(cpu, (res == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, 0);
    _memory_write(cpu->reg.hl, res);
}
// SRL r8
#define SRL_R8(r)                             \
    static void srl_##r(CPU *cpu)             \
    {                                         \
        set_flag_z(cpu, (cpu->reg.r == 0));   \
        set_flag_n(cpu, 0);                   \
        set_flag_h(cpu, 0);                   \
        set_flag_c(cpu, (cpu->reg.r & 0x01)); \
        cpu->reg.r >>= 1;                     \
    }
SRL_R8(a);
SRL_R8(b);
SRL_R8(c);
SRL_R8(d);
SRL_R8(e);
SRL_R8(h);
SRL_R8(l);
// SRL [HL]
static void srl_mhl(CPU *cpu)
{
    uint8_t val = _memory_read(cpu->reg.hl);
    set_flag_z(cpu, (val == 0));
    set_flag_n(cpu, 0);
    set_flag_h(cpu, 0);
    set_flag_c(cpu, (val & 0x01));
    val >>= 1;
    _memory_write(cpu->reg.hl, val);
}
// BIT n, r8
#define BIT_N_R8(n, r)                                   \
    static void bit_##n##_##r(CPU *cpu)                  \
    {                                                    \
        set_flag_z(cpu, ((cpu->reg.r & (1 << n)) == 0)); \
        set_flag_n(cpu, 0);                              \
        set_flag_h(cpu, 1);                              \
    }
BIT_N_R8(0, a);
BIT_N_R8(1, a);
BIT_N_R8(2, a);
BIT_N_R8(3, a);
BIT_N_R8(4, a);
BIT_N_R8(5, a);
BIT_N_R8(6, a);
BIT_N_R8(7, a);
BIT_N_R8(0, b);
BIT_N_R8(1, b);
BIT_N_R8(2, b);
BIT_N_R8(3, b);
BIT_N_R8(4, b);
BIT_N_R8(5, b);
BIT_N_R8(6, b);
BIT_N_R8(7, b);
BIT_N_R8(0, c);
BIT_N_R8(1, c);
BIT_N_R8(2, c);
BIT_N_R8(3, c);
BIT_N_R8(4, c);
BIT_N_R8(5, c);
BIT_N_R8(6, c);
BIT_N_R8(7, c);
BIT_N_R8(0, d);
BIT_N_R8(1, d);
BIT_N_R8(2, d);
BIT_N_R8(3, d);
BIT_N_R8(4, d);
BIT_N_R8(5, d);
BIT_N_R8(6, d);
BIT_N_R8(7, d);
BIT_N_R8(0, e);
BIT_N_R8(1, e);
BIT_N_R8(2, e);
BIT_N_R8(3, e);
BIT_N_R8(4, e);
BIT_N_R8(5, e);
BIT_N_R8(6, e);
BIT_N_R8(7, e);
BIT_N_R8(0, h);
BIT_N_R8(1, h);
BIT_N_R8(2, h);
BIT_N_R8(3, h);
BIT_N_R8(4, h);
BIT_N_R8(5, h);
BIT_N_R8(6, h);
BIT_N_R8(7, h);
BIT_N_R8(0, l);
BIT_N_R8(1, l);
BIT_N_R8(2, l);
BIT_N_R8(3, l);
BIT_N_R8(4, l);
BIT_N_R8(5, l);
BIT_N_R8(6, l);
BIT_N_R8(7, l);
// BIT n, [HL]
#define BIT_N_HL(n)                               \
    static void bit_##n##_mhl(CPU *cpu)           \
    {                                             \
        uint8_t val = _memory_read(cpu->reg.hl);  \
        set_flag_z(cpu, ((val & (1 << n)) == 0)); \
        set_flag_n(cpu, 0);                       \
        set_flag_h(cpu, 1);                       \
    }
BIT_N_HL(0);
BIT_N_HL(1);
BIT_N_HL(2);
BIT_N_HL(3);
BIT_N_HL(4);
BIT_N_HL(5);
BIT_N_HL(6);
BIT_N_HL(7);
// RES n, r8
#define RES_N_R8(n, r)                  \
    static void res_##n##_##r(CPU *cpu) \
    {                                   \
        cpu->reg.r &= ~(1 << n);        \
    }
RES_N_R8(0, a);
RES_N_R8(1, a);
RES_N_R8(2, a);
RES_N_R8(3, a);
RES_N_R8(4, a);
RES_N_R8(5, a);
RES_N_R8(6, a);
RES_N_R8(7, a);
RES_N_R8(0, b);
RES_N_R8(1, b);
RES_N_R8(2, b);
RES_N_R8(3, b);
RES_N_R8(4, b);
RES_N_R8(5, b);
RES_N_R8(6, b);
RES_N_R8(7, b);
RES_N_R8(0, c);
RES_N_R8(1, c);
RES_N_R8(2, c);
RES_N_R8(3, c);
RES_N_R8(4, c);
RES_N_R8(5, c);
RES_N_R8(6, c);
RES_N_R8(7, c);
RES_N_R8(0, d);
RES_N_R8(1, d);
RES_N_R8(2, d);
RES_N_R8(3, d);
RES_N_R8(4, d);
RES_N_R8(5, d);
RES_N_R8(6, d);
RES_N_R8(7, d);
RES_N_R8(0, e);
RES_N_R8(1, e);
RES_N_R8(2, e);
RES_N_R8(3, e);
RES_N_R8(4, e);
RES_N_R8(5, e);
RES_N_R8(6, e);
RES_N_R8(7, e);
RES_N_R8(0, h);
RES_N_R8(1, h);
RES_N_R8(2, h);
RES_N_R8(3, h);
RES_N_R8(4, h);
RES_N_R8(5, h);
RES_N_R8(6, h);
RES_N_R8(7, h);
RES_N_R8(0, l);
RES_N_R8(1, l);
RES_N_R8(2, l);
RES_N_R8(3, l);
RES_N_R8(4, l);
RES_N_R8(5, l);
RES_N_R8(6, l);
RES_N_R8(7, l);
// RES n, [HL]
#define RES_N_HL(n)                              \
    static void res_##n##_mhl(CPU *cpu)          \
    {                                            \
        uint8_t val = _memory_read(cpu->reg.hl); \
        val &= ~(1 << n);                        \
        _memory_write(cpu->reg.hl, val);         \
    }
RES_N_HL(0);
RES_N_HL(1);
RES_N_HL(2);
RES_N_HL(3);
RES_N_HL(4);
RES_N_HL(5);
RES_N_HL(6);
RES_N_HL(7);
// SET n, r8
#define SET_N_R8(n, r)                  \
    static void set_##n##_##r(CPU *cpu) \
    {                                   \
        cpu->reg.r |= (1 << n);         \
    }
SET_N_R8(0, a);
SET_N_R8(1, a);
SET_N_R8(2, a);
SET_N_R8(3, a);
SET_N_R8(4, a);
SET_N_R8(5, a);
SET_N_R8(6, a);
SET_N_R8(7, a);
SET_N_R8(0, b);
SET_N_R8(1, b);
SET_N_R8(2, b);
SET_N_R8(3, b);
SET_N_R8(4, b);
SET_N_R8(5, b);
SET_N_R8(6, b);
SET_N_R8(7, b);
SET_N_R8(0, c);
SET_N_R8(1, c);
SET_N_R8(2, c);
SET_N_R8(3, c);
SET_N_R8(4, c);
SET_N_R8(5, c);
SET_N_R8(6, c);
SET_N_R8(7, c);
SET_N_R8(0, d);
SET_N_R8(1, d);
SET_N_R8(2, d);
SET_N_R8(3, d);
SET_N_R8(4, d);
SET_N_R8(5, d);
SET_N_R8(6, d);
SET_N_R8(7, d);
SET_N_R8(0, e);
SET_N_R8(1, e);
SET_N_R8(2, e);
SET_N_R8(3, e);
SET_N_R8(4, e);
SET_N_R8(5, e);
SET_N_R8(6, e);
SET_N_R8(7, e);
SET_N_R8(0, h);
SET_N_R8(1, h);
SET_N_R8(2, h);
SET_N_R8(3, h);
SET_N_R8(4, h);
SET_N_R8(5, h);
SET_N_R8(6, h);
SET_N_R8(7, h);
SET_N_R8(0, l);
SET_N_R8(1, l);
SET_N_R8(2, l);
SET_N_R8(3, l);
SET_N_R8(4, l);
SET_N_R8(5, l);
SET_N_R8(6, l);
SET_N_R8(7, l);

// SET n, [HL]
#define SET_N_HL(n)                              \
    static void set_##n##_mhl(CPU *cpu)          \
    {                                            \
        uint8_t val = _memory_read(cpu->reg.hl); \
        val |= (1 << n);                         \
        _memory_write(cpu->reg.hl, val);         \
    }
SET_N_HL(0);
SET_N_HL(1);
SET_N_HL(2);
SET_N_HL(3);
SET_N_HL(4);
SET_N_HL(5);
SET_N_HL(6);
SET_N_HL(7);
const opHandler cb_op_table[256] = {
    // 0x00~0x0F
    rlc_b,
    rlc_c,
    rlc_d,
    rlc_e,
    rlc_h,
    rlc_l,
    rlc_mhl,
    rlc_a,
    rrc_b,
    rrc_c,
    rrc_d,
    rrc_e,
    rrc_h,
    rrc_l,
    rrc_mhl,
    rrc_a,
    // 0x10~0x1F
    rl_b,
    rl_c,
    rl_d,
    rl_e,
    rl_h,
    rl_l,
    rl_mhl,
    rla,
    rr_b,
    rr_c,
    rr_d,
    rr_e,
    rr_h,
    rr_l,
    rr_mhl,
    rra,
    // 0x20~0x2F
    sla_b,
    sla_c,
    sla_d,
    sla_e,
    sla_h,
    sla_l,
    sla_mhl,
    sla_a,
    sra_b,
    sra_c,
    sra_d,
    sra_e,
    sra_h,
    sra_l,
    sra_mhl,
    sra_a,
    // 0x30~0x3F
    swap_b,
    swap_c,
    swap_d,
    swap_e,
    swap_h,
    swap_l,
    swap_mhl,
    swap_a,
    srl_b,
    srl_c,
    srl_d,
    srl_e,
    srl_h,
    srl_l,
    srl_mhl,
    srl_a,
    // 0x40~0x4F
    bit_0_b,
    bit_0_c,
    bit_0_d,
    bit_0_e,
    bit_0_h,
    bit_0_l,
    bit_0_mhl,
    bit_0_a,
    bit_1_b,
    bit_1_c,
    bit_1_d,
    bit_1_e,
    bit_1_h,
    bit_1_l,
    bit_1_mhl,
    bit_1_a,
    // 0x50~0x5F
    bit_2_b,
    bit_2_c,
    bit_2_d,
    bit_2_e,
    bit_2_h,
    bit_2_l,
    bit_2_mhl,
    bit_2_a,
    bit_3_b,
    bit_3_c,
    bit_3_d,
    bit_3_e,
    bit_3_h,
    bit_3_l,
    bit_3_mhl,
    bit_3_a,
    // 0x60~0x6F
    bit_4_b,
    bit_4_c,
    bit_4_d,
    bit_4_e,
    bit_4_h,
    bit_4_l,
    bit_4_mhl,
    bit_4_a,
    bit_5_b,
    bit_5_c,
    bit_5_d,
    bit_5_e,
    bit_5_h,
    bit_5_l,
    bit_5_mhl,
    bit_5_a,
    // 0x70~0x7F
    bit_6_b,
    bit_6_c,
    bit_6_d,
    bit_6_e,
    bit_6_h,
    bit_6_l,
    bit_6_mhl,
    bit_6_a,
    bit_7_b,
    bit_7_c,
    bit_7_d,
    bit_7_e,
    bit_7_h,
    bit_7_l,
    bit_7_mhl,
    bit_7_a,
    // 0x80~0x8F
    res_0_b,
    res_0_c,
    res_0_d,
    res_0_e,
    res_0_h,
    res_0_l,
    res_0_mhl,
    res_0_a,
    res_1_b,
    res_1_c,
    res_1_d,
    res_1_e,
    res_1_h,
    res_1_l,
    res_1_mhl,
    res_1_a,
    // 0x90~0x9F
    res_2_b,
    res_2_c,
    res_2_d,
    res_2_e,
    res_2_h,
    res_2_l,
    res_2_mhl,
    res_2_a,
    res_3_b,
    res_3_c,
    res_3_d,
    res_3_e,
    res_3_h,
    res_3_l,
    res_3_mhl,
    res_3_a,
    // 0xA0~0xAF
    res_4_b,
    res_4_c,
    res_4_d,
    res_4_e,
    res_4_h,
    res_4_l,
    res_4_mhl,
    res_4_a,
    res_5_b,
    res_5_c,
    res_5_d,
    res_5_e,
    res_5_h,
    res_5_l,
    res_5_mhl,
    res_5_a,
    // 0xB0~0xBF
    res_6_b,
    res_6_c,
    res_6_d,
    res_6_e,
    res_6_h,
    res_6_l,
    res_6_mhl,
    res_6_a,
    res_7_b,
    res_7_c,
    res_7_d,
    res_7_e,
    res_7_h,
    res_7_l,
    res_7_mhl,
    res_7_a,
    // 0xC0~0xCF
    set_0_b,
    set_0_c,
    set_0_d,
    set_0_e,
    set_0_h,
    set_0_l,
    set_0_mhl,
    set_0_a,
    set_1_b,
    set_1_c,
    set_1_d,
    set_1_e,
    set_1_h,
    set_1_l,
    set_1_mhl,
    set_1_a,
    // 0xD0~0xDF
    set_2_b,
    set_2_c,
    set_2_d,
    set_2_e,
    set_2_h,
    set_2_l,
    set_2_mhl,
    set_2_a,
    set_3_b,
    set_3_c,
    set_3_d,
    set_3_e,
    set_3_h,
    set_3_l,
    set_3_mhl,
    set_3_a,
    // 0xE0~0xEF
    set_4_b,
    set_4_c,
    set_4_d,
    set_4_e,
    set_4_h,
    set_4_l,
    set_4_mhl,
    set_4_a,
    set_5_b,
    set_5_c,
    set_5_d,
    set_5_e,
    set_5_h,
    set_5_l,
    set_5_mhl,
    set_5_a,
    // 0xF0~0xFF
    set_6_b,
    set_6_c,
    set_6_d,
    set_6_e,
    set_6_h,
    set_6_l,
    set_6_mhl,
    set_6_a,
    set_7_b,
    set_7_c,
    set_7_d,
    set_7_e,
    set_7_h,
    set_7_l,
    set_7_mhl,
    set_7_a,

};

// 0xCB : Prefix
static void prefix_cb(CPU *cpu)
{
    uint8_t opcode = _memory_read(cpu->reg.pc);
    cpu->reg.pc++;
    emu->tick_e(emu, 1);
    cb_op_table[opcode](cpu);
}
const opHandler op_table[256] = {
    // 0x00~0x0F

    nop,
    ld_bc_d16,
    ld_mbc_a,
    inc_bc,
    inc_b,
    dec_b,
    ld_b_d8,
    rlca,
    ld_md16_sp,
    add_hl_bc,
    ld_a_mbc,
    dec_bc,
    inc_c,
    dec_c,
    ld_c_d8,
    rrca,
    // 0x10~0x1F
    stop,
    ld_de_d16,
    ld_mde_a,
    inc_de,
    inc_d,
    dec_d,
    ld_d_d8,
    rl_a,
    jr_e8,
    add_hl_de,
    ld_a_mde,
    dec_de,
    inc_e,
    dec_e,
    ld_e_d8,
    rr_a,
    // 0x20~0x2F
    jr_nz_e8,
    ld_hl_d16,
    ld_mhl_a_inc,
    inc_hl,
    inc_h,
    dec_h,
    ld_h_d8,
    daa,
    jr_z_e8,
    add_hl_hl,
    ld_a_mhl_inc,
    dec_hl,
    inc_l,
    dec_l,
    ld_l_d8,
    cpl,
    // 0x30~0x3F
    jr_nc_e8,
    ld_sp_d16,
    ld_mhl_a_dec,
    inc_sp,
    inc_mhl,
    dec_mhl,
    ld_mhl_d8,
    scf,
    jr_c_e8,
    add_hl_sp,
    ld_a_mhl_dec,
    dec_sp,
    inc_a,
    dec_a,
    ld_a_d8,
    ccf,
    // 0x40~0x4F
    ld_b_b,
    ld_b_c,
    ld_b_d,
    ld_b_e,
    ld_b_h,
    ld_b_l,
    ld_b_mhl,
    ld_b_a,
    ld_c_b,
    ld_c_c,
    ld_c_d,
    ld_c_e,
    ld_c_h,
    ld_c_l,
    ld_c_mhl,
    ld_c_a,
    // 0x50~0x5F
    ld_d_b,
    ld_d_c,
    ld_d_d,
    ld_d_e,
    ld_d_h,
    ld_d_l,
    ld_d_mhl,
    ld_d_a,
    ld_e_b,
    ld_e_c,
    ld_e_d,
    ld_e_e,
    ld_e_h,
    ld_e_l,
    ld_e_mhl,
    ld_e_a,
    // 0x60~0x6F
    ld_h_b,
    ld_h_c,
    ld_h_d,
    ld_h_e,
    ld_h_h,
    ld_h_l,
    ld_h_mhl,
    ld_h_a,
    ld_l_b,
    ld_l_c,
    ld_l_d,
    ld_l_e,
    ld_l_h,
    ld_l_l,
    ld_l_mhl,
    ld_l_a,
    // 0x70~0x7F
    ld_mhl_b,
    ld_mhl_c,
    ld_mhl_d,
    ld_mhl_e,
    ld_mhl_h,
    ld_mhl_l,
    halt,
    ld_mhl_a,
    ld_a_b,
    ld_a_c,
    ld_a_d,
    ld_a_e,
    ld_a_h,
    ld_a_l,
    ld_a_mhl,
    ld_a_a,
    // 0x80~0x8F
    add_a_b,
    add_a_c,
    add_a_d,
    add_a_e,
    add_a_h,
    add_a_l,
    add_a_mhl,
    add_a_a,
    adc_a_b,
    adc_a_c,
    adc_a_d,
    adc_a_e,
    adc_a_h,
    adc_a_l,
    adc_a_mhl,
    adc_a_a,
    // 0x90~0x9F
    sub_a_b,
    sub_a_c,
    sub_a_d,
    sub_a_e,
    sub_a_h,
    sub_a_l,
    sub_a_mhl,
    sub_a_a,
    sbc_a_b,
    sbc_a_c,
    sbc_a_d,
    sbc_a_e,
    sbc_a_h,
    sbc_a_l,
    sbc_a_mhl,
    sbc_a_a,
    // 0xA0~0xAF
    and_a_b,
    and_a_c,
    and_a_d,
    and_a_e,
    and_a_h,
    and_a_l,
    and_a_mhl,
    and_a_a,
    xor_a_b,
    xor_a_c,
    xor_a_d,
    xor_a_e,
    xor_a_h,
    xor_a_l,
    xor_a_mhl,
    xor_a_a,
    // 0xB0~0xBF
    or_a_b,
    or_a_c,
    or_a_d,
    or_a_e,
    or_a_h,
    or_a_l,
    or_a_mhl,
    or_a_a,
    cp_a_b,
    cp_a_c,
    cp_a_d,
    cp_a_e,
    cp_a_h,
    cp_a_l,
    cp_a_mhl,
    cp_a_a,
    // 0xC0~0xCF
    ret_nz,
    pop_bc,
    jp_nz_a16,
    jp_a16,
    call_nz_a16,
    push_bc,
    add_a_d8,
    rst_0x0000,
    ret_z,
    ret,
    jp_z_a16,
    prefix_cb,
    call_z_a16,
    call_a16,
    adc_a_d8,
    rst_0x0008,
    // 0xD0~0xDF
    ret_nc,
    pop_de,
    jp_nc_a16,
    nop,
    call_nc_a16,
    push_de,
    sub_a_d8,
    rst_0x0010,
    ret_c,
    reti,
    jp_c_a16,
    nop,
    call_c_a16,
    nop,
    sbc_a_d8,
    rst_0x0018,
    // 0xE0~0xEF
    ldh_md8_a,
    pop_hl,
    ldh_mc_a,
    nop,
    nop,
    push_hl,
    and_a_d8,
    rst_0x0020,
    add_sp_e8,
    jp_mhl,
    ld_md16_a,
    nop,
    nop,
    nop,
    xor_a_d8,
    rst_0x0028,
    // 0xF0~0xFF
    ldh_a_md8,
    pop_af,
    ldh_a_mc,
    di,
    nop,
    push_af,
    or_a_d8,
    rst_0x0030,
    ld_hl_sp_e8,
    ld_sp_hl,
    ld_a_md16,
    ei,
    nop,
    nop,
    cp_a_d8,
    rst_0x0038,
};

#ifdef DEBUG
const char *op_table_names[256] = {
    "nop",
    "ld_bc_d16",
    "ld_mbc_a",
    "inc_bc",
    "inc_b",
    "dec_b",
    "ld_b_d8",
    "rlca",
    "ld_md16_sp",
    "add_hl_bc",
    "ld_a_mbc",
    "dec_bc",
    "inc_c",
    "dec_c",
    "ld_c_d8",
    "rrca",
    // 0x10~0x1F
    "stop",
    "ld_de_d16",
    "ld_mde_a",
    "inc_de",
    "inc_d",
    "dec_d",
    "ld_d_d8",
    "rl_a",
    "jr_e8",
    "add_hl_de",
    "ld_a_mde",
    "dec_de",
    "inc_e",
    "dec_e",
    "ld_e_d8",
    "rr_a",
    // 0x20~0x2F
    "jr_nz_e8",
    "ld_hl_d16",
    "ld_mhl_a_inc",
    "inc_hl",
    "inc_h",
    "dec_h",
    "ld_h_d8",
    "daa",
    "jr_z_e8",
    "add_hl_hl",
    "ld_a_mhl_inc",
    "dec_hl",
    "inc_l",
    "dec_l",
    "ld_l_d8",
    "cpl",
    // 0x30~0x3F
    "jr_nc_e8",
    "ld_sp_d16",
    "ld_mhl_a_dec",
    "inc_sp",
    "inc_mhl",
    "dec_mhl",
    "ld_mhl_d8",
    "scf",
    "jr_c_e8",
    "add_hl_sp",
    "ld_a_mhl_dec",
    "dec_sp",
    "inc_a",
    "dec_a",
    "ld_a_d8",
    "ccf",
    // 0x40~0x4F
    "ld_b_b",
    "ld_b_c",
    "ld_b_d",
    "ld_b_e",
    "ld_b_h",
    "ld_b_l",
    "ld_b_mhl",
    "ld_b_a",
    "ld_c_b",
    "ld_c_c",
    "ld_c_d",
    "ld_c_e",
    "ld_c_h",
    "ld_c_l",
    "ld_c_mhl",
    "ld_c_a",
    // 0x50~0x5F
    "ld_d_b",
    "ld_d_c",
    "ld_d_d",
    "ld_d_e",
    "ld_d_h",
    "ld_d_l",
    "ld_d_mhl",
    "ld_d_a",
    "ld_e_b",
    "ld_e_c",
    "ld_e_d",
    "ld_e_e",
    "ld_e_h",
    "ld_e_l",
    "ld_e_mhl",
    "ld_e_a",
    // 0x60~0x6F
    "ld_h_b",
    "ld_h_c",
    "ld_h_d",
    "ld_h_e",
    "ld_h_h",
    "ld_h_l",
    "ld_h_mhl",
    "ld_h_a",
    "ld_l_b",
    "ld_l_c",
    "ld_l_d",
    "ld_l_e",
    "ld_l_h",
    "ld_l_l",
    "ld_l_mhl",
    "ld_l_a",
    // 0x70~0x7F
    "ld_mhl_b",
    "ld_mhl_c",
    "ld_mhl_d",
    "ld_mhl_e",
    "ld_mhl_h",
    "ld_mhl_l",
    "halt",
    "ld_mhl_a",
    "ld_a_b",
    "ld_a_c",
    "ld_a_d",
    "ld_a_e",
    "ld_a_h",
    "ld_a_l",
    "ld_a_mhl",
    "ld_a_a",
    // 0x80~0x8F
    "add_a_b",
    "add_a_c",
    "add_a_d",
    "add_a_e",
    "add_a_h",
    "add_a_l",
    "add_a_mhl",
    "add_a_a",
    "adc_a_b",
    "adc_a_c",
    "adc_a_d",
    "adc_a_e",
    "adc_a_h",
    "adc_a_l",
    "adc_a_mhl",
    "adc_a_a",
    // 0x90~0x9F
    "sub_a_b",
    "sub_a_c",
    "sub_a_d",
    "sub_a_e",
    "sub_a_h",
    "sub_a_l",
    "sub_a_mhl",
    "sub_a_a",
    "sbc_a_b",
    "sbc_a_c",
    "sbc_a_d",
    "sbc_a_e",
    "sbc_a_h",
    "sbc_a_l",
    "sbc_a_mhl",
    "sbc_a_a",
    // 0xA0~0xAF
    "and_a_b",
    "and_a_c",
    "and_a_d",
    "and_a_e",
    "and_a_h",
    "and_a_l",
    "and_a_mhl",
    "and_a_a",
    "xor_a_b",
    "xor_a_c",
    "xor_a_d",
    "xor_a_e",
    "xor_a_h",
    "xor_a_l",
    "xor_a_mhl",
    "xor_a_a",
    // 0xB0~0xBF
    "or_a_b",
    "or_a_c",
    "or_a_d",
    "or_a_e",
    "or_a_h",
    "or_a_l",
    "or_a_mhl",
    "or_a_a",
    "cp_a_b",
    "cp_a_c",
    "cp_a_d",
    "cp_a_e",
    "cp_a_h",
    "cp_a_l",
    "cp_a_mhl",
    "cp_a_a",
    // 0xC0~0xCF
    "ret_nz",
    "pop_bc",
    "jp_nz_a16",
    "jp_a16",
    "call_nz_a16",
    "push_bc",
    "add_a_d8",
    "rst_0x0000",
    "ret_z",
    "ret",
    "jp_z_a16",
    "prefix_cb",
    "call_z_a16",
    "call_a16",
    "adc_a_d8",
    "rst_0x0008",
    // 0xD0~0xDF
    "ret_nc",
    "pop_de",
    "jp_nc_a16",
    "nop",
    "call_nc_a16",
    "push_de",
    "sub_a_d8",
    "rst_0x0010",
    "ret_c",
    "reti",
    "jp_c_a16",
    "nop",
    "call_c_a16",
    "nop",
    "sbc_a_d8",
    "rst_0x0018",
    // 0xE0~0xEF
    "ldh_md8_a",
    "pop_hl",
    "ldh_mc_a",
    "nop",
    "nop",
    "push_hl",
    "and_a_d8",
    "rst_0x0020",
    "add_sp_e8",
    "jp_mhl",
    "ld_md16_a",
    "nop",
    "nop",
    "nop",
    "xor_a_d8",
    "rst_0x0028",
    // 0xF0~0xFF
    "ldh_a_md8",
    "pop_af",
    "ldh_a_mc",
    "di",
    "nop",
    "push_af",
    "or_a_d8",
    "rst_0x0030",
    "ld_hl_sp_e8",
    "ld_sp_hl",
    "ld_a_md16",
    "ei",
    "nop",
    "nop",
    "cp_a_d8",
    "rst_0x0038",
};
#endif // debug