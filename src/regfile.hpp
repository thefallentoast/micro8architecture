#ifndef REGFILEHPP
#define REGFILEHPP
#include <cstdint>

using i8 = int8_t;
using u8 = uint8_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;

typedef struct {
    u32 eax;   // \-
    u32 ebx;   // | 4 General purpose registers
    u32 ecx;   // |
    u32 edx;   // /-

    u32 si;    // Source and dest registers for fast copying
    u32 di;    // addresses

    u32 ds;    // \-
    u32 cs;    // | 4 Segment registers
    u32 ss;    // |
    u32 es;    // /-

    u32 pc;    // } Program counter
    u32 ir;    // } Instruction register
    u32 flags; // } Flags register
} regfile_t;

// Basic functions to interact with a register file
void regfile_init(regfile_t &regfile);
u32* regfile_seek(regfile_t &regfile, int reg);
void regfile_writ(regfile_t &regfile, u32 value, int reg, int byt = 0);
u32  regfile_read(regfile_t &regfile, int reg, int byt = 0);
u32  regfile_ginc(regfile_t &regfile);
u32  regfile_gtpc(regfile_t &regfile);

#endif