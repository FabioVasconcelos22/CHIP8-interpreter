#include "chip8.h"
#include <iostream>
#include <cmath>

//#define DEBUG_MODE

chip8::chip8(keyboard & keyboard, bool shift_quirk, bool load_store_quirk) :
    _keyboard { &keyboard},
    _shift_quirk {shift_quirk},
    _load_store_quirk {load_store_quirk}
{
    _ram.write(font.data(), font.size(), chip8_constant::FONT_START_ADDR);
}

bool chip8::load_rom(const std::string& rom_path) {
    FILE* fp = fopen(rom_path.c_str(), "r");
    if (!fp) {
        return false;
    }

    std::array <uint8_t, chip8_constant::MEMORY_SIZE> (buffer);
    size_t read_size = fread(buffer.data(), 1, buffer.size() ,fp);
    if(read_size == 0) {
        return false;
    }

    _ram.write(buffer.data(), read_size, chip8_constant::PROGRAM_START_ADDR);

    fclose(fp);
    return true;
}

void chip8::interpret_instruction (uint16_t const & inst) {

#ifdef DEBUG_MODE
    std::cout << std::endl << "Command: " << std::hex << (inst >> 12) << std::endl;
#endif

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

#ifdef DEBUG_MODE
    for (int i = 0; i < 16; i+=4) {
        for (int j=0; j<4; ++j) {
            std::cout << "V[" << i+j << "] = " << (int)_registers[i+j] << "     ";
        }
        std::cout << std::endl;
    }

    std::cout << "Program counter: " << _program_counter << std::endl;
    std::cout << "index register: " << _index_register << std::endl;

    auto tmp_stack = _stack;
    while (!tmp_stack.empty()) {
        std::cout << "stack: " << tmp_stack.top() << std::endl;
        tmp_stack.pop();
    }
#endif
}

void chip8::update() {
    if (_draw) {
        _draw = false;
    }

    auto instruction = _ram.read <uint16_t> (_program_counter);
    interpret_instruction (instruction);
}

void chip8::interpret_0_group(uint16_t const & inst) {
    switch (inst & 0x00FF) {
        case 0x00E0: {
            _pixels.fill(OFF_COLOR);
            _draw = true;
            }
#ifdef DEBUG_MODE
    std::cout << "|_____0x0E0" << std::endl;
#endif
            break;
        case 0x00EE:
            if (!_stack.empty()) {
                _program_counter = _stack.top();
                _stack.pop();
            }
#ifdef DEBUG_MODE
    std::cout << "|_____0x0EE" << std::endl;
#endif
            break;
        default:
            std::cout << "Unsupported command" << std::endl;
            exit(1);
    }
    _program_counter += 2;
}

void chip8::interpret_1_group(const uint16_t &inst) {
    _program_counter = inst & 0x0FFF;
}

void chip8::interpret_2_group(const uint16_t &inst) {
    _stack.push(_program_counter);
    _program_counter = inst & 0X0FFF;
}

void chip8::interpret_3_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t NN = inst & 0x00FF;

    if (_registers[VX] == NN) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_4_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t NN = inst & 0x00FF;

    if (_registers[VX] == NN) {
        _program_counter += 2;
    } else {
        _program_counter += 4;
    }
}


void chip8::interpret_5_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t VY = (inst & 0x00F0) >> 4;

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
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint16_t NN = inst & 0x00FF;

    _registers[VX] += NN;
    _program_counter += 2;
}

void chip8::interpret_8_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t VY = (inst & 0x00F0) >> 4;
    uint8_t x = inst & 0x000F;

#ifdef DEBUG_MODE
    std::cout << "|_____0x" << (int)x;
#endif

    switch (x) {
        case 0:
            _registers[VX] = _registers[VY];
            break;
        case 0x01:
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
            _registers[VX] = sum;
            _registers[0x0F] = (sum > 255) ? 1 : 0;
            break;
        }
        case 0x05: {
            int diff = _registers[VX] - _registers[VY];
            _registers[VX] = diff;
            _registers[0x0F] = (diff < 0) ? 0 : 1;
            break;
        }
        case 0x06:
            if (_shift_quirk) {
                // VX = VX >> 1
                bool flag = _registers[VX] % 2;
                _registers[VX] = _registers[VX] >> 1;
                _registers[0x0F] = flag;
            } else {
                // VX = VY >> 1
                bool flag = _registers[VY] % 2;
                _registers[VX] = _registers[VY] >> 1;
                _registers[0x0F] = flag;
            }
            break;
        case 0x07: {
            int diff = _registers[VY] - _registers[VX];
            _registers[VX] = diff;
            _registers[0x0F] = (diff < 0) ? 0 : 1;
            break;
        }
        case 0x0E:
            if (_shift_quirk) {
                // VX = VX << 1
                bool flag = _registers[VX] >> 7;
                _registers[VX] = _registers[VX] << 1;
                _registers[0x0F] = flag;
            } else {
                // VX = VY << 1
                bool flag = _registers[VY] >> 7;
                _registers[VX] = _registers[VY] << 1;
                _registers[0x0F] = flag;
            }
            break;
        default:
            std::cout << "wrong interpreter command" << std::endl;
    }
    _program_counter += 2;
}

void chip8::interpret_9_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t VY = (inst & 0x00F0) >> 4;

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
    _program_counter = _registers[0] + (inst & 0x0FFF);
}

void chip8::interpret_C_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t NN = inst & 0x00FF;

    _registers[VX] = rand() % 255 & NN;
    _program_counter += 2;
}

void chip8::interpret_D_group(const uint16_t &inst) {
    uint8_t startX = (inst & 0x0F00) >> 8;
    uint8_t startY = (inst & 0x00F0) >> 4;
    uint8_t height = inst & 0x000F;

    _registers[0x0F] = 0;

    for (int y = 0; y < height; ++y) {
        uint8_t sprite_row {};

        if (_index_register + y > _ram.size()) {
            return;
        }
        sprite_row = _ram.read <uint8_t> (_index_register + y);
        for (int x = 0; x < 8; ++x) {
            if (sprite_row & (0x80 >> x)) {
                int index =
                        (_registers[startX] + x) % chip8_constant::DISPLAY_WIDTH +
                        (_registers[startY] + y) % chip8_constant::DISPLAY_HEIGHT * chip8_constant::DISPLAY_WIDTH;

                if (_pixels[index] == ON_COLOR) {
                    _registers[0x0F] = 1;
                    _pixels[index] = OFF_COLOR;
                    _draw = true;

                } else {
                    _pixels[index] = ON_COLOR;
                    _draw = true;
                }
            }
        }
    }
    _program_counter += 2;
}
void chip8::interpret_E_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t sub_inst = inst & 0x00FF;

    uint8_t key_pressed = 0xFF;

    for (int i = 0; i < 16; ++i) {
        if (_keyboard->get_key_value(i)) {
            key_pressed = i;
            break;
        }
    }
#ifdef DEBUG_MODE
    std::cout << "|_____0x" << std::hex << (int)sub_inst << std::endl;
#endif
    switch (sub_inst) {
        case 0x9E:
            if ( key_pressed == _registers [VX]) {
                _program_counter += 2;
            }
            break;
        case 0xA1:
            if ( key_pressed != _registers [VX]) {
                _program_counter += 2;
            }
            break;
        default:
            std::cout << "Invalid sub instruction" << std::endl;
    }
    _program_counter += 2;
}

void chip8::interpret_F_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t sub_inst = inst & 0x00FF;

#ifdef DEBUG_MODE
    std::cout << "|_____0x" << std::hex << (int)sub_inst << std::endl;
#endif
    switch (sub_inst) {
        case 0x07:
            _registers[VX] = delay;
            break;
        case 0x0A: {
            auto key_pressed = false;
            for (int i = 0; i < 16; ++i) {
                if (_keyboard->get_key_value(i)) {
                    _registers[VX] = i;
                    key_pressed = true;
                    break;
                }
            }

            if (!key_pressed) {
                return; // return here to continue in infinite cycle till a key is pressed
            }
            break;
        }
        case 0x15:
            delay = _registers[VX];
            break;
        case 0x18:
            sound = _registers[VX];
            break;
        case 0x1E:
            _index_register += _registers[VX];
            break;
        case 0x29:
            _index_register = chip8_constant::FONT_START_ADDR + _registers[VX] * 5; //5 bytes per font character
            break;
        case 0x33: {
            uint16_t decimal_number = _registers[VX];
            std::array <uint8_t, 3> data {};

            data.at(2) = decimal_number % 10;
            decimal_number /= 10;

            data.at(1) = decimal_number % 10;
            decimal_number /= 10;

            data.at(0) = decimal_number % 10;

            _ram.write( &data.at(0), 3, _index_register);
        }
            break;
        case 0x55:
            for (uint8_t i = 0; i <= VX; ++i) {
                _ram.write( &_registers[i], 1, _index_register + i);
            }

            // if not quirk, also set I = I + X + 1
            if (!_load_store_quirk) {
                _index_register += VX + 1;
            }
            break;
        case 0x65: {
            for (uint8_t i = 0; i <= VX; ++i) {
                _registers[i] = _ram.read <uint8_t> (_index_register+i);
            }
            // if not quirk, also set I = I + X + 1
            if (!_load_store_quirk) {
                _index_register += VX + 1;
            }
            break;
        }
        default:
            std::cout << "Unsupported command" << std::endl;
    }
    _program_counter += 2;
}
