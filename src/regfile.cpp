#include <cstdint>
#include "regfile.hpp"

static const u32 mask[4] = {0xFFFFFFFF, 0xFFFFFF00, 0xFFFF00FF, 0xFFFF0000};
#define shift(byt) (byt == 2) * 8

void regfile_init(regfile_t &regfile) {
    regfile.eax = 0; // GP registers start at 0
    regfile.ebx = 0;
    regfile.ecx = 0;
    regfile.edx = 0;

    regfile.si = 0;  // Source index
    regfile.di = 0;  // Dest index

    regfile.ds = 0x0001;   // Data segment
    regfile.cs = 0x0FFF;   // Code segment at last 64kb (256MiB address map -> 0xFFFFFF0)
    regfile.ss = 0x0000;   // Stack segment
    regfile.es = 0x0001;   // Extra segment

    regfile.pc = 0xFFEF;   // Program counter starts at 256MiB - 16, but it has to start 1 lower for regfile_cinc to work
    regfile.ir = 0x0000;   // HLT instruction
    regfile.flags = 0;     // Nothing on
    return;
}

/*
    regfile is the regfile (duh).
    reg is an index from 0 to 11, representing the register in the register file.
*/
inline u32* regfile_seek(regfile_t &regfile, int reg) {
    return &regfile.eax + reg;
}

/*
    regfile is, well, the regfile.
    reg is an index from 0 to 11, representing the register in the appropriate regfile.
    byt is one of:
        0: entire register
        1: lower 8 bits
        2: higher 8 bits of lower 16 bits
        3: lower 16 bits
*/
void regfile_writ(regfile_t &regfile, u32 value, int reg, int byt) {
    // 1. obtain pointer to the register
    u32 *regfile_register = regfile_seek(regfile, reg);
    // 2. write
    *regfile_register = (reg & mask[byt]) | ((value & ~mask[byt]) >> shift(byt));
    return;
}

/*
    regfile is the regfile (again).
    reg is an index from 0 to 11, representing the register in the register file.
    byt is one of:
        0: entire register
        1: lower 8 bits
        2: higher 8 bits of lower 16 bits
        3: lower 16 bits
*/
u32 regfile_read(regfile_t &regfile, int reg, int byt) {
    // 1. obtain value of the register
    u32 regfile_register = *regfile_seek(regfile, reg);
    // 2. read
    return (regfile_register & mask[byt]) >> shift(byt);
}

/*
    this will increment and return the program counter
    regfile is the regfile (i am tired of this)
*/
u32 regfile_ginc(regfile_t &regfile) {
    // just increment the program counter
    return ++regfile.pc;
}

/*
    this will get you the program counter
    regfile is the regfile (self-explanatory)
*/
u32 regfile_gtpc(regfile_t &regfile) {
    return regfile.pc;
}