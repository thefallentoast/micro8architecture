#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <error.h>

#include "regfile.hpp"
#include "fde.hpp"

int main(int argc, const char** argv) {
    std::cout << "ToastBIOS v0.1\n";
    if (argc < 1+1) {
        std::cerr << "Not enough arguments";
        exit(1);
    }
    regfile_t regfile;
    regfile_init(regfile);

    u8* memory = new u8[0x10000000]; // 256MiB

    // Initialise all of memory to 0
    for (int i = 0; i < 0x10000000; i += 512 * 1024) {
        for (int j = 0; j < 512 * 1024; j++) {
            memory[i+j] = 0;
        }
        std::cout << "\r" << i / 1000 << "K OK" << std::flush; // \r moves cursor to beginning of line
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
    std::cout << "\n";

    size_t fill_size = 64 * 1024;

    // Copy the file to the last 64k
    std::ifstream bios(argv[1], std::ios::binary | std::ios::ate);
    if (bios.tellg() != 0x10000) {
        std::cerr << "BIOS too small\n";
        exit(1);
    };
    bios.seekg(0, std::ios::beg);

    u8 tempbuf[0x10000];
    if (bios.is_open()) {
       if (!bios.read(reinterpret_cast<char*>(tempbuf), 0x10000)) {
            std::cerr << "Could not read bios\n";
            return 1;
       }
    }
    std::memcpy(memory + 0x10000000 - fill_size, tempbuf, fill_size);

    // Now we start executing things. TODO
    while (!(regfile.flags & FLAGS::STOP)) {
        execute(regfile, memory);
    }
    delete[] memory;
    return 0;
}