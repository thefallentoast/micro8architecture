#include <iostream>
#include <iomanip>
#include <cstdint>
#include "fde.hpp"

u8 nextbyt(regfile_t &regfile, u8* mem) {
    u32 pc = regfile_ginc(regfile);
    u8 byt = mem[(pc & 0xFFFF) | (regfile.cs << 16)];
    /* USED PREVIOUSLY FOR DEBUGGING 
    std::cout << "Read byte 0x" 
              << std::hex
              << std::setw(2) << std::setfill('0')
              << static_cast<int>(byt)
              << " at 0x" 
              << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(pc)
              << std::endl; */
    return byt;
}

void execute(regfile_t &regfile, u8* mem) {
    // 1. fetch (get instruction)
    regfile.ir = nextbyt(regfile, mem);
    // CI = compact instructions, EI = extended instructions (two-byte at least)
    // 2. decode (CI or EI)
    bool CIorEI = (regfile.ir & 0xF0) == 0; // True if EI
    bool implied = (regfile.ir >> 4) == 0;  // True if in the implieds block
    // 3. execute
    if (implied) {
        // Implied block executes first
        switch (regfile.ir) {
            // TODO
            default: {
                break;
            }
        }
    } else if (CIorEI) {
        // Run EI before CI
        u8 instrbyte = nextbyt(regfile, mem);
        switch (instrbyte | ((regfile.ir & 0xF) << 4)) {
            // TODO
            default: {
                break;
            }
        }
    } else {
        // CI decode
        switch (regfile.ir >> 4) {
            // TODO
            default: {
                break;
            }
        }
    }
    return;
}

/*void execute(regfile_t &regfile, u8* mem) {
    // 1. fetch (get instruction)
    regfile.ir = nextbyt(regfile, mem);
    // 2,3. decode and execute
    
    switch((regfile.ir & 0b11100000) >> 5) {
        case INSTRUCTION::MOV: {
            // Register bytes are obtained from the first byte
            int byt1 = (regfile.ir >> 2) & 0b11;
            int byt2 = regfile.ir & 0b11;
            // Register indexes are obtained from the second byte
            u8 indexes = nextbyt(regfile, mem);
            int idx1 = indexes >> 4;
            int idx2 = indexes & 0b1111;
            // Get the register
            u32 value = regfile_read(regfile, idx1, byt1);
            // Write
            regfile_writ(regfile, value, idx2, byt2);
            break;
    }
        case INSTRUCTION::ONEBYT: {
            // Nothing
        } [[fallthrough]]; // This should fall through
        default: {
            break;
    }
    }
    switch (regfile.ir) {
        case INSTRUCTION::HALT: {
            regfile.flags |= FLAGS::STOP;
            break;
        }
        case INSTRUCTION::JMPNEAR: {
            // Move maximum of 128 bytes, signed
            int8_t value = (int8_t)nextbyt(regfile, mem);
            // Actually move
            regfile.pc += value;
            break;
        }
        case INSTRUCTION::OUT_SHORT: {
            u8 outport = nextbyt(regfile, mem);
            u8 data = nextbyt(regfile, mem);
            if (outport == 1) {
                std::cout << static_cast<char>(data);
            }
            break;
        }
    }
}*/