#include "serial.h"
#include "emu.h"

#include "ir.h"
rQueue serial_buffer;
extern Emulator *emu;

static inline bool is_master_mode(Serial *s)
{
    return (s->sc & 0b0000001) == 0;
}
static inline bool is_enabled(Serial *s)
{
    return (s->sc & 0b1000000) == 0;
}

void serial_init(Serial *s)
{
    s->sb = 0xFF;
    s->sc = 0x7C;
    s->out_byte = 0;
    s->out_bit = 0;
    s->transfering = false;
    rQueue_init(&serial_buffer, 512);
}

static inline void serial_start_transfer(Serial *s)
{
    s->transfering = true;
    s->out_byte = s->sb;
    s->out_bit = 7;
}

static inline void serial_stop_transfer(Serial *s)
{
    rQueue_push(&serial_buffer, s->out_byte);
    s->transfering = false;
    // sc bit 7
    s->sc &= 0x7F;
    // irq
    emu->IF |= IR_SERIAL_MASK;
}

static inline void serial_process(Serial *s)
{
    if (s->out_bit == 0)
    {
        serial_stop_transfer(s);
        return;
    }
    s->sb <<= 1;
    ++s->sb; // set the lowest bit 1
    s->out_bit--;
}
void serial_tick(Serial *s)
{
    if (!s->transfering && is_enabled(s) && is_master_mode(s))
    {
        // start transfer
        serial_start_transfer(s);
    }
    else if (s->transfering)
    {
        // process transfer
        serial_process(s);
    }
}