#include "chip8.h"
#include <iostream>

chip8::chip8() {
    mem.write(font, FONT_SIZE, FONT_ADRR);

    program_counter = START_PROGRAM_ADDRESS;
}

void chip8::interpret_instruction (uint16_t const & inst) {
    switch (inst >> 12) {
        case 0:
            interpret_0_group(inst);
            break;
        case 1:
            interpret_1_group(inst);
            break;
        case 2:
            interpret_2_group(inst);
            break;
        case 3:
            interpret_3_group(inst);
            break;
        case 4:
            interpret_4_group(inst);
            break;
        case 5:
            interpret_5_group(inst);
            break;
        case 6:
            interpret_6_group(inst);
            break;
        case 7:
            interpret_7_group(inst);
            break;
        case 8:
            interpret_8_group(inst);
            break;
        case 9:
            interpret_9_group(inst);
            break;
        case 10:
            interpret_A_group(inst);
            break;
        case 11:
            interpret_B_group(inst);
            break;
        case 12:
            interpret_C_group(inst);
            break;
        case 13:
            interpret_D_group(inst);
            break;
        case 14:
            interpret_E_group(inst);
            break;
        case 15:
            interpret_F_group(inst);
            break;
        default:
            std::cout << "Unsupported instruction" << std::endl;

    }
}

void chip8::update() {
    uint16_t instruction = mem.read(program_counter);
    interpret_instruction (instruction);

    // TODO interpret instruction here
}

bool chip8::load_rom(const std::string& rom_path) {
    FILE* fp = fopen(rom_path.c_str(), "r");
    if (!fp) {
        return false;
    }

    uint8_t buffer [4096];
    size_t read_size = fread(buffer, 1, 4096 ,fp);
    if(read_size == 0) {
        return false;
    }

    mem.write(buffer, read_size, START_PROGRAM_ADDRESS);

    fclose(fp);
    return true;
}

void chip8::interpret_0_group(uint16_t const & inst) {
    switch (inst & 0x00FF) {
        case 0x00E0:
            // TODO Clear the display by setting all pixels to ‘off’.
            program_counter += 2;
            break;
        case 0x00EE:
            if (stack.empty()) {
                std::cout << "stack is empty" << std::endl;
                break;
            }
            program_counter = stack.top();
            stack.pop();
            break;
        default:
            std::cout << "Unsupported command" << std::endl;
    }
}

void chip8::interpret_1_group(const uint16_t &inst) {
    program_counter = inst & 0x0FFF;
}

void chip8::interpret_2_group(const uint16_t &inst) {
    stack.push(program_counter + 2);
    program_counter = inst & 0X0FFF;
}

void chip8::interpret_3_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t NN = inst & 0x00FF;

    if ( index_register[VX] == NN) {
        program_counter += 4;
    } else {
        program_counter += 2;
    }
}

void chip8::interpret_4_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t NN = inst & 0x00FF;

    if ( index_register[VX] == NN) {
        program_counter += 2;
    } else {
        program_counter += 4;
    }
}


void chip8::interpret_5_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t VY = inst & 0x00F0 >> 4;

    if ( index_register[VX] == index_register[VY]) {
        program_counter += 4;
    } else {
        program_counter += 2;
    }
}

void chip8::interpret_6_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint16_t NN = inst & 0x00FF;

    index_register[VX] = NN;

    program_counter += 2;
}

void chip8::interpret_7_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint16_t NN = inst & 0x00FF;

    index_register[VX] += NN;

    program_counter += 2;
}

void chip8::interpret_8_group(const uint16_t &inst) {
    uint8_t VX = 0x0F00 >> 8;
    uint8_t VY = 0x00F0 >> 4;
    uint8_t x = 0x000F;

    switch (x) {
        case 0:
            index_register[VX] = index_register[VY];
            break;
        case 0x01:
            index_register[VX] |= index_register[VY];
            break;
        case 0x02:
            index_register[VX] &= index_register[VY];
            break;
        case 0x03:
            index_register[VX] ^= index_register[VY];
            break;
        case 0x04: {
            uint16_t sum = index_register[VX] + index_register[VY];
            variable[0x0F] = (sum > 256) ? 1 : 0;
            index_register[VX] += index_register[VY];
            break;
        }
        case 0x05: {
            uint16_t diff = index_register[VX] - index_register[VY];
            variable[0x0F] = (diff < 0) ? 1 : 0;
            index_register[VX] -= index_register[VY];
            break;
        }
        case 0x06:
            variable[0x0F] = index_register[VX] % 2;
            index_register[VX] = index_register[VX] >> 1;
            break;
        case 0x07: {
            uint16_t diff = index_register[VY] - index_register[VX];
            variable[0x0F] = (diff < 0) ? 1 : 0;
            index_register[VX] = index_register[VY] - index_register[VX];
            break;
        }
        case 0x0E:
            variable[0x0F] = index_register[VX] >> 7;
            index_register[VX] = index_register[VX] << 1;
        default:
            std::cout << "wrong interpreter command" << std::endl;
    }
    program_counter += 2;
}

void chip8::interpret_9_group(const uint16_t &inst) {
    uint8_t VX = 0x0F00 >> 8;
    uint8_t VY = 0x00F0 >> 4;

    if (index_register[VX] != index_register[VY]) {
        program_counter += 4;
    } else {
        program_counter += 2;
    }
}

void chip8::interpret_A_group(const uint16_t &inst) {
    index_address = inst & 0x0FFF;
    program_counter += 2;
}

void chip8::interpret_B_group(const uint16_t &inst) {
    program_counter = variable[0] + inst & 0x0FFF;
}

void chip8::interpret_C_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t NN = inst & 0x00FF;

    index_register[VX] = rand() % 256 & NN;
}

void chip8::interpret_D_group(const uint16_t &inst) {
    // TODO Draw to display
}

void chip8::interpret_E_group(const uint16_t &inst) {
    // TODO Keys
}

void chip8::interpret_F_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t sub_inst = inst & 0x00FF;

    // TODO Implement this
    switch (sub_inst) {
        case 0x07:
            break;
        case 0x0A:
            break;
        case 0x15:
            break;
        case 0x18:
            break;
        case 0x1E:
            break;
        case 0x29:
            break;
        case 0x33:
            break;
        case 0x55:
            break;
        case 0x65:
            break;
        default:
            std::cout << "Unsupported command" << std::endl;
    }
}
