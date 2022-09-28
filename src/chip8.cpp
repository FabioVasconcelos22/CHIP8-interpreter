#include "chip8.h"
#include <iostream>
#include <cmath>

#define DEBUG_MODE

chip8::chip8(keyboard & keyboard) :
    _keyboard { & keyboard}
{
    _ram.write(font, FONT_SIZE, FONT_START_ADDR);
    _program_counter = PROGRAM_START_ADDR;
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

void chip8::interpret_instruction (uint16_t const & inst) {
    #ifdef DEBUG_MODE
        std::cout << "Command: " << std::hex << (inst >> 12) << " : ";
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
}

void chip8::update() {
    if (_draw) {
        _display.draw(_pixels[0]);
        _draw = false;
    }

    uint16_t instruction = _ram.read_2bytes(_program_counter);
    interpret_instruction (instruction);
}

void chip8::interpret_0_group(uint16_t const & inst) {
    switch (inst & 0x00FF) {
        case 0x00E0: {
            memset(_pixels, OFF_COLOR, sizeof(_pixels));
            _draw = true;
            }
#ifdef DEBUG_MODE
    std::cout << "0x0E0: Clear the screen" << std::endl;
#endif
            break;
        case 0x00EE:
            if (!_stack.empty()) {
                _program_counter = _stack.top();
                _stack.pop();
            }
#ifdef DEBUG_MODE
    std::cout << "0x0EE: pop from stack PC:" << _program_counter << std::endl;
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
#ifdef DEBUG_MODE
    std::cout << "0x1xx: set PC to:" << _program_counter << std::endl;
#endif
}

void chip8::interpret_2_group(const uint16_t &inst) {
    _stack.push(_program_counter);
#ifdef DEBUG_MODE
    std::cout << "0x2xx: Stores current PC " << _program_counter << " into stack. ";
#endif
    _program_counter = inst & 0X0FFF;
#ifdef DEBUG_MODE
    std::cout << "Than jumps PC to " << _program_counter << std::endl;
#endif
}

void chip8::interpret_3_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t NN = inst & 0x00FF;

#ifdef DEBUG_MODE
    std::cout << "if " << _registers[VX] << " == " << NN << " jumps next opcode" << std::endl;
#endif

    if (_registers[VX] == NN) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_4_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t NN = inst & 0x00FF;

#ifdef DEBUG_MODE
    std::cout << "if " << _registers[VX] << " != " << NN << " jumps next opcode" << std::endl;
#endif

    if (_registers[VX] == NN) {
        _program_counter += 2;
    } else {
        _program_counter += 4;
    }
}


void chip8::interpret_5_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t VY = (inst & 0x00F0) >> 4;

#ifdef DEBUG_MODE
    std::cout << "if " << (int)_registers[VX] << " == " << (int)_registers[VY] << " jumps next opcode" << std::endl;
#endif

    if (_registers[VX] == _registers[VY]) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_6_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint16_t NN = inst & 0x00FF;

#ifdef DEBUG_MODE
    std::cout << "Set register " << (int)VX << " to " << NN << std::endl;
#endif

    _registers[VX] = NN;

    _program_counter += 2;
}

void chip8::interpret_7_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint16_t NN = inst & 0x00FF;

    _registers[VX] += NN;

#ifdef DEBUG_MODE
    std::cout << "Sum " << (int)NN << " to register " << (int)VX << ". Register[VX] = " << (int)_registers[VX] << std::endl;
#endif
    _program_counter += 2;
}

void chip8::interpret_8_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t VY = (inst & 0x00F0) >> 4;
    uint8_t x = inst & 0x000F;

#ifdef DEBUG_MODE
    std::cout << (int)x;
#endif

    switch (x) {
        case 0:
            _registers[VX] = _registers[VY];

#ifdef DEBUG_MODE
            std::cout << " Register " << (int)VX << " = Register "<< (int)VY << ". Result : " << (int)_registers[VX]  << std::endl;
#endif
            break;
        case 0x01:
            _registers[VX] |= _registers[VY];
#ifdef DEBUG_MODE
            std::cout << " Register " << VX << " | Register "<< VY << ". Result : " << _registers[VX]  << std::endl;
#endif
            break;
        case 0x02:
            _registers[VX] &= _registers[VY];
#ifdef DEBUG_MODE
            std::cout << " Register " << VX << " & Register "<< VY << ". Result : " << _registers[VX]  << std::endl;
#endif
            break;
        case 0x03:
            _registers[VX] ^= _registers[VY];
#ifdef DEBUG_MODE
            std::cout << " Register " << VX << "  Register "<< VY << ". Result : " << _registers[VX]  << std::endl;
#endif
            break;
        case 0x04: {
            uint16_t sum = _registers[VX] + _registers[VY];
            _registers[VX] += _registers[VY];
            _registers[0x0F] = (sum > 255) ? 1 : 0;
#ifdef DEBUG_MODE
            std::cout << " Register " << (int)VX << " + Register "<< (int)VY << ". Result : " << (int)_registers[VX] << ", Overflow: " << (int)_registers[0x0F] << std::endl;
#endif
            break;
        }
        case 0x05: {
            int diff = _registers[VX] - _registers[VY];
            _registers[0x0F] = (diff < 0) ? 0 : 1;
            _registers[VX] = diff;
#ifdef DEBUG_MODE
            std::cout << " Register " << (int)VX << " - Register "<< (int)VY << ". Result : " << (int)_registers[VX] << ", Overflow: " << (int)_registers[0x0F] << std::endl;
#endif
            break;
        }
        case 0x06: {
            bool flag = _registers[VX] % 2;
            _registers[VX] = _registers[VX] >> 1;
            _registers[0x0F] = flag;
#ifdef DEBUG_MODE
            std::cout << " Register " << (int) VX << " right shift. Result : " << (int) _registers[VX] << ", Overflow: "
                      << (int) _registers[0x0F] << std::endl;
#endif
            }
            break;
        case 0x07: {
            int diff = _registers[VY] - _registers[VX];
            _registers[0x0F] = (diff < 0) ? 0 : 1;
            _registers[VX] = diff;
#ifdef DEBUG_MODE
            std::cout << " Register " << (int)VX << " = Register "<< (int)VY << " - Register " << (int)VX << " . Result : " << (int)_registers[VX] << ", Overflow: " << (int)_registers[0x0F] << std::endl;
#endif
            break;
        }
        case 0x0E: {
            bool flag = _registers[VX] >> 7;
            _registers[VX] = _registers[VX] << 1;
            _registers[0x0F] = flag;
#ifdef DEBUG_MODE
            std::cout << " Register " << VX << " left shift. Result : " << _registers[VX] << ", Overflow: " << _registers[0x0F] << std::endl;
#endif
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

#ifdef DEBUG_MODE
    std::cout << "if " << _registers[VX] << " != " << _registers[VY] << " jumps next opcode" << std::endl;
#endif

    if (_registers[VX] != _registers[VY]) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_A_group(const uint16_t &inst) {
    _index_register = inst & 0x0FFF;
    _program_counter += 2;

#ifdef DEBUG_MODE
    std::cout << "index register =  " << _index_register << std::endl;
#endif
}

void chip8::interpret_B_group(const uint16_t &inst) {
    _program_counter = _registers[0] + (inst & 0x0FFF);
#ifdef DEBUG_MODE
    std::cout << "PC =  " <<_program_counter << std::endl;
#endif
}

void chip8::interpret_C_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t NN = inst & 0x00FF;

    _registers[VX] = rand() % 255 & NN;

#ifdef DEBUG_MODE
    std::cout << "Random number to Register "<< VX << ".Result: " << _registers[VX] << std::endl;
#endif

    _program_counter += 2;
}

void chip8::interpret_D_group(const uint16_t &inst) {
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t VY = (inst & 0x00F0) >> 4;
    uint8_t VN = inst & 0x000F;

    _registers[0x0F] = 0;

    for (int y = 0; y < VN; ++y) {
        uint8_t sprite_row {};
        _ram.read(&sprite_row, 1, _index_register + y);
        for (int x = 0; x < 8; x++) {
            if (sprite_row & (0x80) >> x) {
                int index =
                        (_registers[VX] + x) % DISPLAY_WIDTH +
                        (_registers[VY] + y) % DISPLAY_HEIGHT * DISPLAY_WIDTH;
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
#ifdef DEBUG_MODE
    std::cout << "Draw things" << std::endl;
#endif
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

    switch (sub_inst) {
        case 0x9E:
            if ( key_pressed == _registers [VX]) {
#ifdef DEBUG_MODE
                std::cout << "if " << (int)key_pressed << " == " << (int)_registers [VX] << " jumps next instruction" << std::endl;
#endif
                _program_counter += 2;
            }
            break;
        case 0xA1:
#ifdef DEBUG_MODE
            std::cout << "if " << (int)key_pressed << " != " << (int)_registers [VX] << "jumps next instruction" << std::endl;
#endif
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

    switch (sub_inst) {
#ifdef DEBUG_MODE
        std::cout << (int)sub_inst;
#endif
        case 0x07:
            _registers[VX] = delay;
#ifdef DEBUG_MODE
            std::cout << "Register " << VX << " = " << delay << std::endl;
#endif
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
#ifdef DEBUG_MODE
            std::cout << "Key was been pressed, jump to next opcode" << std::endl;
#endif
            break;
        }
        case 0x15:
            delay = _registers[VX];
#ifdef DEBUG_MODE
            std::cout << "Delay = Register " << VX << ". Result: " << delay << std::endl;
#endif
            break;
        case 0x18:
            sound = _registers[VX];
#ifdef DEBUG_MODE
            std::cout << "Sound = Register " << VX << ". Result: " << sound << std::endl;
#endif
            break;
        case 0x1E:
            _index_register += _registers[VX];
#ifdef DEBUG_MODE
            std::cout << "index register += Register " << VX << ". Result: " << _index_register << std::endl;
#endif
            break;
        case 0x29:
            _index_register = _registers[VX] * 0x5; //5 pixels per sprite
#ifdef DEBUG_MODE
            std::cout << "index register = Register " << (int)VX << "*5. Result: " << _index_register << std::endl;
#endif
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

#ifdef DEBUG_MODE
            std::cout << "0x33 ";
            for (int i = 0; i < 3;++i) {
                std::cout << "data [" << i << "] = " << (int)data.at(i) << ", ";
            }
            std::cout << std::endl;
#endif
        }
            break;
        case 0x55:
            for (uint8_t i = 0; i <= VX; ++i) {
                _ram.write( &_registers[i], 1, _index_register + i);
#ifdef DEBUG_MODE
                std::cout << "write register [" << i << "] = " << _index_register + i << ", ";
#endif
            }

            break;
        case 0x65: {
#ifdef DEBUG_MODE
            std::cout << "0x65 ";
#endif
            for (uint8_t i = 0; i <= VX; ++i) {
                _ram.read( &_registers[i], 1, _index_register + i);
#ifdef DEBUG_MODE
                std::cout << "Read register [" << (int)i << "] = " << _index_register + i << ", ";
#endif
            }
#ifdef DEBUG_MODE
            std::cout << std::endl;
#endif
            break;
        }
        default:
            std::cout << "Unsupported command" << std::endl;
    }
    _program_counter += 2;
}
