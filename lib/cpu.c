#include "cpu.h"
#include "common.h"
#include "mem.h"
#include "instruction.h"
#include "emu.h"
#include "ir.h"
extern Emulator *emu;
#ifdef DEBUG
extern FILE *log_file;

#endif
static int reset(CPU *cpu)
{
    printf("reset cpu\n");
    cpu->reg.af = 0x01B0;
    cpu->reg.bc = 0x0013;
    cpu->reg.de = 0x00D8;
    cpu->reg.hl = 0x014D;
    cpu->reg.sp = 0xFFFE;
    cpu->reg.pc = 0x0100;
    cpu->ime = false;
    cpu->halted = false;
    // reset interrupt
    emu->IF = 0;
    emu->IE = 0;
    return 0;
}

static void step(CPU *cpu)
{
    // ir check: ime、ie、if
    if (cpu->ime && (emu->IE & emu->IF))
    {
        ir_handel(cpu);
    }

    // read opcode
    OpCode opcode = memory_read(cpu->reg.pc);
    emu->tick_e(emu, 1);
#ifdef DEBUG
    static int cpu_i;

    cpu_i++;
    if (cpu_i >= 10000)
    {
        // stop cpu
        emu->running = false;
    }
    char flags[16];
    sprintf(flags, "%c%c%c%c",
            cpu->reg.f & (1 << 7) ? 'Z' : '-',
            cpu->reg.f & (1 << 6) ? 'N' : '-',
            cpu->reg.f & (1 << 5) ? 'H' : '-',
            cpu->reg.f & (1 << 4) ? 'C' : '-');

    // printf("%08lX - %04X: %-12s (%02X %02X %02X) A: %02X F: %s BC: %02X%02X DE: %02X%02X HL: %02X%02X  \n",
    //        emu->clock_cycles, cpu->reg.pc, op_table_names[opcode], opcode,
    //        memory_read(cpu->reg.pc + 1), memory_read(cpu->reg.pc + 2), cpu->reg.a, flags, cpu->reg.b, cpu->reg.c,
    //        cpu->reg.d, cpu->reg.e, cpu->reg.h, cpu->reg.l);

    fprintf(log_file, "%04X: (%02X %02X %02X) A: %02X F: %s BC: %02X%02X DE: %02X%02X HL: %02X%02X\n",
            cpu->reg.pc, opcode,
            memory_read(cpu->reg.pc + 1), memory_read(cpu->reg.pc + 2), cpu->reg.a, flags, cpu->reg.b, cpu->reg.c,
            cpu->reg.d, cpu->reg.e, cpu->reg.h, cpu->reg.l);

    fflush(log_file); // 确保日志立即写入文件
#endif
    cpu->reg.pc++;
    // execute opcode
    op_table[opcode](cpu);
}

int cpu_init(CPU *cpu)
{
    printf("init cpu...\n");
    cpu->reg.af = 0x01B0;
    cpu->reg.bc = 0x0013;
    cpu->reg.de = 0x00D8;
    cpu->reg.hl = 0x014D;
    cpu->reg.sp = 0xFFFE;
    cpu->reg.pc = 0x0100;
    cpu->ime = false;
    cpu->halted = false;
    cpu->reset = reset;
    cpu->step = step;
    return 0;
}
