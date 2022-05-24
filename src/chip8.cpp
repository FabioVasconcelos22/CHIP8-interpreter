#include "chip8.h"
#include <iostream>

chip8::chip8() {
    mem.write(font, FONT_SIZE, FONT_ADRR);

    program_counter = START_PROGRAM_ADDRESS;
}

void chip8::interpret_instruction (uint16_t inst) {
    switch (inst & 0xF000) {
        case 0x0000:
            interpret_0_group(inst);
            break;
        case 0x1000:
            //jump to address in NNN
            //program_counter = reinterpret_cast<uint16_t *>(inst & 0x0FFF);
            break;
        case 0x2000:
            //Call the subroutine at address NNN. It increments SP, puts the current PC at the top of the stack and sets PC to the address NNN
            break;
        case 0x3000:
            break;
        case 0x4000:
            break;
        case 0x5000:
            break;
        case 0x6000:
            break;
        case 0x7000:
            break;
        case 0x8000:
            break;
        case 0x9000:
            break;
        case 0xA000:
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
    }
}

void chip8::interpret_0_group(uint16_t inst) {
    switch (inst & 0xFFF) {
        case 0x00E0:
            // TODO Clear the display by setting all pixels to ‘off’.
            break;
        case 0x00EE:
            // TODO Return from a subroutine. Pops the value at the top of the stack (indicated by the stack pointer SP) and puts it in PC.
            break;
        default:
            std::cout << "Unsupported command" << std::endl;
    }
}

void chip8::update() {
    uint16_t instruction = mem.read(program_counter);
    program_counter ++;

    // TODO interpret instruction here
}

bool chip8::load_rom(const std::string& rom_path) {
    FILE* fp = fopen(rom_path.c_str(), "r");
    if (!fp) {
        return false;
    }

    uint16_t buffer [4096];
    size_t read_size = fread(buffer, sizeof(*buffer), 4096 ,fp);
    if(read_size == 0) {
        return false;
    }

    mem.write(buffer, read_size, START_PROGRAM_ADDRESS);

    fclose(fp);
    return true;
}
