#ifndef FDEHPP
#define FDEHPP

#include "regfile.hpp"

#define SETFLAG(flags, flag, cond) (flags & ~flag) | (cond * flag)

u8   nextbyt(regfile_t &regfile, u8* mem);
void execute(regfile_t &regfile, u8* mem);

enum INSTRUCTION : u8 {
    // TODO: add more instructions
    // CI (Compact Instructions) block; 12-bits parameter
    INC = 0x1, // \ These instructions get special treatment as they only use the 4 bits of parameter, not 12.
    DEC = 0x2, // /
    JMPSC = 0x3, // 4-bit condition, 8-bit distance
    MOVIMMLO = 0x5, // 4-bit reg, 8-bit imm
    

    // Implieds
    HLT = 0x0,
    OUT = 0x1,
    IN  = 0x2,
    NOP = 0xF
};

enum FLAGS : u32 {
    // TODO: add more flags
    STOP = 1U << 31,

    ZERO       = 1U << 0,
    CARRY      = 1U << 1,
    OVERFLOW   = 1U << 2
};

enum CONDITIONS : u8 {
    AL = 0x0,  // Jump
    ZF = 0x1,  // Jump if zero flag set
    NE = 0x2,  // Jump if negative (MSB set)
    CR = 0x3,  // Jump if carry set
    EQ = 0x4,  // Jump if equal
    GT = 0x5,  // Jump if greater than
    GE = 0x6,  // Jump if greater than or equal
    VF = 0x7, // oVerflow flag set

    NOT = 0b1000 // NOT flag
};

#endif