#include <iostream>
#include <iomanip>
#include <cstdint>
#include "fde.hpp"

u8 nextbyt(regfile_t &regfile, u8* mem) {
    u32 pc = regfile_ginc(regfile);
    u8 byt = mem[(pc & 0xFFFF) | (regfile.cs << 16)];
    /*USED PREVIOUSLY FOR DEBUGGING 
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

inline bool check_cond(regfile_t &regfile, u8* mem) {
    bool flip = (regfile.ir & 0x8) >> 3;
    switch (regfile.ir & 0x7) {
        case CONDITIONS::AL: {
            return true ^ flip;
        }
        case CONDITIONS::ZF: {
            return (regfile.flags & FLAGS::ZERO) ^ flip;
        }
    }
    return true;
}

void execute(regfile_t &regfile, u8* mem) {
    // 1. fetch (get instruction)
    regfile.ir = nextbyt(regfile, mem);
    // CI = compact instructions, EI = extended instructions (two-byte at least)
    // 2. decode (CI or EI)
    bool CIorEI = (regfile.ir & 0xF0) == 0xF0; // True if EI
    bool implied = (regfile.ir >> 4) == 0;  // True if in the implieds block
    //std::cout << "IR: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)regfile.ir << std::endl;
    // 3. execute
    if (implied) {
        // Implied block executes first
        switch (regfile.ir) {
            // TODO
            case INSTRUCTION::HLT: {
                regfile.flags |= FLAGS::STOP;
                break;
            }
            case INSTRUCTION::OUT: {
                u16 outport = regfile.edx & 0xFFFF;
                u8 data = regfile.eax & 0xFF;
                //std::cout << "OUT: " << std::hex << (int)outport << " " << std::hex << (int)data << std::endl;
                if (outport == 1) {
                    // Print to stdout
                    std::cout << data;
                    break;
                }
                // TODO: Write to output file
            }
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
            case INSTRUCTION::INC: {
                u32 *reg = regfile_seek(regfile, regfile.ir & 0xF);
                ++*reg; // Just increment the value
                regfile.flags = SETFLAG(regfile.flags, FLAGS::CARRY | FLAGS::ZERO, *reg == 0);
                regfile.flags = SETFLAG(regfile.flags, FLAGS::OVERFLOW, *reg == 0x80000000);
            }
            case INSTRUCTION::DEC: {
                --*regfile_seek(regfile, regfile.ir & 0xF); // Just decrement the value
            }
            case INSTRUCTION::JMPSC: {
                // Condition is regfile.ir & 0xF;
                if (check_cond(regfile, mem)) {
                    regfile.pc += (i8)nextbyt(regfile, mem); // Get the value to jump from the next byte    
                }
                break;
            }
            case INSTRUCTION::MOVIMMLO: {
                u8 imm = nextbyt(regfile, mem); 
                u8 reg = regfile.ir & 0xF;
                //std::cout << "MOV: " << std::hex << (int)regfile.ir << " " << std::hex << (int)imm << std::endl;
                // Write to register
                regfile_writ(regfile, imm, reg, 1); // Always to L
                break;
            }
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