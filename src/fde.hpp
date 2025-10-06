#ifndef FDEHPP
#define FDEHPP

#include "regfile.hpp"

u8   nextbyt(regfile_t &regfile, u8* mem);
void execute(regfile_t &regfile, u8* mem);

enum INSTRUCTION : u8 {
    // TODO: add more instructions
    TWOBYT = 0b1110,
    MOV = 0x1,
    MOVIMM = 0x2,
    ADD = 0x3,
    ADDIMM = 0x4,

    ONEBYT = 0x00,
    JMPNEAR = 0xE1,
    OUT_SHORT = 0xE2,
    HALT = 0
};

enum FLAGS : u32 {
    // TODO: add more flags
    STOP = 1U << 31
};

#endif