#include <iostream>
#include "fde.hpp"

u8 nextbyt(regfile_t &regfile, u8* mem) {
    u32 pc = regfile_ginc(regfile);
    return mem[pc | (regfile.cs << 16)];
}

void execute(regfile_t &regfile, u8* mem) {
    // 1. fetch (get instruction)
    regfile.ir = nextbyt(regfile, mem);
    // 2,3. decode and execute
    switch (regfile.ir) {
        case INSTRUCTION::HALT:
            regfile.flags |= FLAGS::STOP;
            break;
        case INSTRUCTION::OUT_SHORT:
            u8 outport = nextbyt(regfile, mem);
            u8 data = nextbyt(regfile, mem);
            if (outport == 1) {
                std::cout << static_cast<char>(data);
            }
            break;
    }
}