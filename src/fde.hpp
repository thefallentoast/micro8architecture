#ifndef FDEHPP
#define FDEHPP

#include "regfile.hpp"

u8   nextbyt(regfile_t &regfile, u8* mem);
void execute(regfile_t &regfile, u8* mem);

enum INSTRUCTION : u8 {
    // TODO: add more instructions
    OUT_SHORT = 0x52,
    HALT = 0
};

enum FLAGS : u32 {
    // TODO: add more flags
    STOP = 1U << 31
};

#endif