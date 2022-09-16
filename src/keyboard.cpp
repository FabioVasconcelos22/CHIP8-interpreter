#include "keyboard.h"
#include <cctype>

bool keyboard::get_key_value(uint8_t const & index) const {
    return _keys [index];
}

void keyboard::set_key(uint8_t const & key) {
    uint8_t index = ascii_to_chip8_key (key);
    _keys [index] = true;
}

void keyboard::clear_key(uint8_t const & key) {
    uint8_t index = ascii_to_chip8_key (key);
    _keys [index] = false;
}

uint8_t keyboard::ascii_to_chip8_key(const uint8_t &key) {
    switch (std::tolower(key)) {
        case '1':
            return 0x01;
        case '2':
            return 0x02;
        case '3':
            return 0x03;
        case '4':
            return 0x0C;
        case 'q':
            return 0x04;
        case 'w':
            return 0x05;
        case 'e':
            return 0x06;
        case 'r':
            return 0x0D;
        case 'a':
            return 0x07;
        case 's':
            return 0x08;
        case 'd':
            return 0x09;
        case 'f':
            return 0x0E;
        case 'z':
            return 0x0A;
        case 'x':
            return 0x00;
        case 'c':
            return 0x0B;
        case 'v':
            return 0x0F;
        default:
            return 0xFF;
    }
}
