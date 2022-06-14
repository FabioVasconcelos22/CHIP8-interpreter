#include "chip8.h"
#include <iostream>
#include <cmath>

chip8::chip8() {
    _ram.write(font, FONT_SIZE, FONT_START_ADDR);

    _program_counter = PROGRAM_START_ADDR;
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
    uint8_t * inst_buffer {};
    uint16_t instruction = _ram.read_2bytes(_program_counter);
    interpret_instruction (instruction);
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

    _ram.write(buffer, read_size, PROGRAM_START_ADDR);

    fclose(fp);
    return true;
}

void chip8::interpret_0_group(uint16_t const & inst) {
    switch (inst & 0x00FF) {
        case 0x00E0: {
            _display.clear();

            int display_size = DISPLAY_WIDTH * DISPLAY_HEIGHT;
            uint8_t buffer [display_size];
            _ram.write(buffer, display_size, DISPLAY_START_ADDR);

            _program_counter += 2; 
        }
            break;
        case 0x00EE:
            if (_stack.empty()) {
                std::cout << "_stack is empty" << std::endl;
                break;
            }
            _program_counter = _stack.top();
            _stack.pop();
            break;
        default:
            std::cout << "Unsupported command" << std::endl;
    }
}

void chip8::interpret_1_group(const uint16_t &inst) {
    _program_counter = inst & 0x0FFF;
}

void chip8::interpret_2_group(const uint16_t &inst) {
    _stack.push(_program_counter + 2);
    _program_counter = inst & 0X0FFF;
}

void chip8::interpret_3_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t NN = inst & 0x00FF;

    if (_registers[VX] == NN) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_4_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t NN = inst & 0x00FF;

    if (_registers[VX] == NN) {
        _program_counter += 2;
    } else {
        _program_counter += 4;
    }
}


void chip8::interpret_5_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t VY = inst & 0x00F0 >> 4;

    if (_registers[VX] == _registers[VY]) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_6_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint16_t NN = inst & 0x00FF;

    _registers[VX] = NN;

    _program_counter += 2;
}

void chip8::interpret_7_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint16_t NN = inst & 0x00FF;

    _registers[VX] += NN;

    _program_counter += 2;
}

void chip8::interpret_8_group(const uint16_t &inst) {
    uint8_t VX = 0x0F00 >> 8;
    uint8_t VY = 0x00F0 >> 4;
    uint8_t x = 0x000F;

    switch (x) {
        case 0:
            _registers[VX] = _registers[VY];
            break;
        case 0x01:
            _registers[VX] |= _registers[VY];
            _registers[VX] |= _registers[VY];
            break;
        case 0x02:
            _registers[VX] &= _registers[VY];
            break;
        case 0x03:
            _registers[VX] ^= _registers[VY];
            break;
        case 0x04: {
            uint16_t sum = _registers[VX] + _registers[VY];
            _registers[0x0F] = (sum > 256) ? 1 : 0;
            _registers[VX] += _registers[VY];
            break;
        }
        case 0x05: {
            uint16_t diff = _registers[VX] - _registers[VY];
            _registers[0x0F] = (diff < 0) ? 1 : 0;
            _registers[VX] -= _registers[VY];
            break;
        }
        case 0x06:
            _registers[0x0F] = _registers[VX] % 2;
            _registers[VX] = _registers[VX] >> 1;
            break;
        case 0x07: {
            uint16_t diff = _registers[VY] - _registers[VX];
            _registers[0x0F] = (diff < 0) ? 1 : 0;
            _registers[VX] = _registers[VY] - _registers[VX];
            break;
        }
        case 0x0E:
            _registers[0x0F] = _registers[VX] >> 7;
            _registers[VX] = _registers[VX] << 1;
        default:
            std::cout << "wrong interpreter command" << std::endl;
    }
    _program_counter += 2;
}

void chip8::interpret_9_group(const uint16_t &inst) {
    uint8_t VX = 0x0F00 >> 8;
    uint8_t VY = 0x00F0 >> 4;

    if (_registers[VX] != _registers[VY]) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_A_group(const uint16_t &inst) {
    _index_register = inst & 0x0FFF;
    _program_counter += 2;
}

void chip8::interpret_B_group(const uint16_t &inst) {
    _program_counter = _registers[0x00] + inst & 0x0FFF;
}

void chip8::interpret_C_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t NN = inst & 0x00FF;

    _registers[VX] = rand() % 256 & NN;

    _program_counter += 2;
}

void chip8::interpret_D_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t VY = (inst & 0x00F0) >> 4;
    uint8_t VN = inst & 0x000F;

    _registers[0x0F] = 0;

    int x_pos = _registers[VX] % DISPLAY_WIDTH;
    int y_pos = _registers[VY] % DISPLAY_HEIGHT;

    for (int row = 0; row < VN ; ++row) {
        uint8_t sprite_row {};
        _ram.read(&sprite_row, 1, _index_register + row);

        uint8_t screen_byte {};
        _ram.read(&screen_byte,
                  1,
                  DISPLAY_START_ADDR + x_pos + ((y_pos + row) * DISPLAY_WIDTH)
                  );

        for (int bit = 0; bit < 8; ++bit) {
            if ((sprite_row & (0x80 >> bit)) != 0 ) {
                _registers[0x0F] = 1;
            }
            screen_byte ^= 1;
        }
        _ram.write( &screen_byte, 1, DISPLAY_START_ADDR + x_pos + (y_pos * DISPLAY_WIDTH));
    }

    _program_counter += 2;
}
void chip8::interpret_E_group(const uint16_t &inst) {
    // TODO Keys
}

void chip8::interpret_F_group(const uint16_t &inst) {
    uint8_t VX = inst & 0x0F00 >> 8;
    uint8_t sub_inst = inst & 0x00FF;

    uint8_t * buffer;

    // TODO Implement this
    switch (sub_inst) {
        case 0x07:
            _registers[VX] = _delay;
            break;
        case 0x0A:
            // TODO Keys
            break;
        case 0x15:
            _delay = _registers[VX];
            break;
        case 0x18:
            _sound = _registers[VX];
            break;
        case 0x1E:
            _index_register += _registers[VX];
            break;
        case 0x29:
            _index_register = _registers[VX];
            break;
        case 0x33: {
            uint16_t decimal_number = _registers[VX];
            uint16_t * ptr = &_index_register;

            *ptr = decimal_number * static_cast<uint16_t>(pow(10, -3));
            ptr[1] = decimal_number * static_cast<uint16_t>(pow(10, -2));
            ptr[2] = decimal_number * static_cast<uint16_t>(pow(10, -1));
        }
            break;
        case 0x55: {
            uint16_t * ptr = &(_index_register);
            for (uint16_t i = 0; i < VX ; ++i) {
                ptr[i] = _registers[i];
            }
        }
            break;
        case 0x65:
            for (uint8_t i; i < VX; ++i) {
                _registers[i] = _ram.read_2bytes(_index_register + i);
            }
            break;
        default:
            std::cout << "Unsupported command" << std::endl;
    }
    _program_counter += 2;
}
