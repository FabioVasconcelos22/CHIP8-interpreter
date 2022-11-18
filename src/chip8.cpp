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

    _timestamp = std::chrono::system_clock::now ();
    _cpu_timestamp = std::chrono::system_clock::now();
    _timers_timestamp = std::chrono::system_clock::now();
}

bool chip8::load_rom(const std::string& rom_path) {
    std::unique_ptr<FILE> file (fopen(rom_path.c_str(), "r"));

    if (!file) {
        return false;
    }

    std::array <uint8_t, chip8_constant::MEMORY_SIZE> (buffer);
    size_t read_size = fread(buffer.data(), 1, buffer.size(), file.get());

    if(read_size == 0) {
        return false;
    }

    _ram.write(buffer.data(), read_size, chip8_constant::PROGRAM_START_ADDR);
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

void chip8::run_instruction() {
    auto instruction = _ram.read <uint16_t> (_program_counter);
    interpret_instruction (instruction);
}

void chip8::interpret_0_group(uint16_t const & inst) {
    switch (inst & 0x00FF) {
        case 0x00E0: {
            // Clears the screen.
            _pixels.fill(OFF_COLOR);
            _draw = true;
            }
#ifdef DEBUG_MODE
    std::cout << "|_____0x0E0" << std::endl;
#endif
            break;

        case 0x00EE:
            // Returns from a subroutine.
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
    // Jumps to address NNN.
    _program_counter = inst & 0x0FFF;
}

void chip8::interpret_2_group(const uint16_t &inst) {
    // Calls subroutine at NNN.
    _stack.push(_program_counter);
    _program_counter = inst & 0X0FFF;
}

void chip8::interpret_3_group(const uint16_t &inst) {
    // Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block);
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t NN = inst & 0x00FF;

    if (_registers[VX] == NN) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_4_group(const uint16_t &inst) {
    // Skips the next instruction if VX does not equal NN. (Usually the next instruction is a jump to skip a code block);
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t NN = inst & 0x00FF;

    if (_registers[VX] == NN) {
        _program_counter += 2;
    } else {
        _program_counter += 4;
    }
}


void chip8::interpret_5_group(const uint16_t &inst) {
    // Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block);
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t VY = (inst & 0x00F0) >> 4;

    if (_registers[VX] == _registers[VY]) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_6_group(const uint16_t &inst) {
    // Sets VX to NN.
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint16_t NN = inst & 0x00FF;

    _registers[VX] = NN;
    _program_counter += 2;
}

void chip8::interpret_7_group(const uint16_t &inst) {
    // Adds NN to VX. (Carry flag is not changed);
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
            // Sets VX to the value of VY.
            _registers[VX] = _registers[VY];
            break;
        case 0x01:
            // Sets VX to VX or VY. (Bitwise OR operation);
            _registers[VX] |= _registers[VY];
            break;
        case 0x02:
            // Sets VX to VX and VY. (Bitwise AND operation);
            _registers[VX] &= _registers[VY];
            break;
        case 0x03:
            // Sets VX to VX xor VY.
            _registers[VX] ^= _registers[VY];
            break;
        case 0x04: {
            // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not.
            uint16_t sum = _registers[VX] + _registers[VY];
            _registers[VX] = sum;
            _registers[0x0F] = (sum > 255) ? 1 : 0;
            break;
        }
        case 0x05: {
            // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not.
            int diff = _registers[VX] - _registers[VY];
            _registers[VX] = diff;
            _registers[0x0F] = (diff < 0) ? 0 : 1;
            break;
        }
        case 0x06:
            // Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
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
            // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
            int diff = _registers[VY] - _registers[VX];
            _registers[VX] = diff;
            _registers[0x0F] = (diff < 0) ? 0 : 1;
            break;
        }
        case 0x0E:
            // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
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
    // Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block);
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t VY = (inst & 0x00F0) >> 4;

    if (_registers[VX] != _registers[VY]) {
        _program_counter += 4;
    } else {
        _program_counter += 2;
    }
}

void chip8::interpret_A_group(const uint16_t &inst) {
    // Sets I to the address NNN.
    _index_register = inst & 0x0FFF;
    _program_counter += 2;
}

void chip8::interpret_B_group(const uint16_t &inst) {
    // Jumps to the address NNN plus V0.
    _program_counter = _registers[0] + (inst & 0x0FFF);
}

void chip8::interpret_C_group(const uint16_t &inst) {
    // Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
    uint8_t VX = (inst & 0x0F00) >> 8;
    uint8_t NN = inst & 0x00FF;

    _registers[VX] = rand() % 255 & NN;
    _program_counter += 2;
}

void chip8::interpret_D_group(const uint16_t &inst) {
    // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
    // Each row of 8 pixels is read as bit-coded starting from memory location I;
    // I value does not change after the execution of this instruction.
    // As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
    // and to 0 if that does not happen.

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
            // Skips the next instruction if the key stored in VX is pressed.
            // (Usually the next instruction is a jump to skip a code block);
            if ( key_pressed == _registers [VX]) {
                _program_counter += 2;
            }
            break;
        case 0xA1:
            // Skips the next instruction if the key stored in VX is not pressed.
            // (Usually the next instruction is a jump to skip a code block);
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
            // Sets VX to the value of the _delay timer.
            _registers[VX] = _delay;
            break;
        case 0x0A: {
            // A key press is awaited, and then stored in VX.
            // (Blocking Operation. All instruction halted until next key event);
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
            // Sets the _delay timer to VX.
            _delay = _registers[VX];
            break;
        case 0x18:
            // Sets the _sound timer to VX.
            _sound = _registers[VX];
            break;
        case 0x1E:
            // Adds VX to I. VF is not affected.
            _index_register += _registers[VX];
            break;
        case 0x29:
            // Sets I to the location of the sprite for the character in VX.
            // Characters 0-F (in hexadecimal) are represented by a 4x5 font.
            _index_register = chip8_constant::FONT_START_ADDR + _registers[VX] * 5; //5 bytes per font character
            break;
        case 0x33: {
            // Stores the binary-coded decimal representation of VX, with the most significant of three digits
            // at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.
            // (In other words, take the decimal representation of VX, place the hundreds digit in memory at
            // location in I, the tens digit at location I+1, and the ones digit at location I+2.);
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
            // Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is
            // increased by 1 for each value written, but I itself is left unmodified.
            for (uint8_t i = 0; i <= VX; ++i) {
                _ram.write( &_registers[i], 1, _index_register + i);
            }

            // if not load store quirk, also set I = I + X + 1
            if (!_load_store_quirk) {
                _index_register += VX + 1;
            }
            break;
        case 0x65: {
            // Fills from V0 to VX (including VX) with values from memory, starting at address I.
            // The offset from I is increased by 1 for each value read, but I itself is left unmodified.
            for (uint8_t i = 0; i <= VX; ++i) {
                _registers[i] = _ram.read <uint8_t> (_index_register+i);
            }
            // if not load store quirk, also set I = I + X + 1
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

void chip8::update(display_interface & display, sound_interface & speakers) {
    if ((std::chrono::system_clock::now () - _cpu_timestamp) >= chip8_constant::CPU_FRAME_RATE) {
        _cpu_timestamp = std::chrono::system_clock::now ();
        if (_draw) {
            display.draw( *_pixels.data() );
            _draw = false;
        }
        run_instruction();
    }

    if ((std::chrono::system_clock::now () - _timers_timestamp) >= chip8_constant::TIMERS_FRAME_RATE) {
        if (_delay > 0) {
            _delay--;
        }

        if (_sound > 0) {
            speakers.play();
            _sound--;

            if (_sound <= 0) {
                speakers.stop();
            }
        }
        _timers_timestamp = std::chrono::system_clock::now();
    }
}
