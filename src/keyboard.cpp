#include "inputs/keyboard.h"
#include <cctype>

keyboard::keyboard(std::map<char, uint16_t>  & key_map) :
    _keyboard {key_map} { }

bool keyboard::get_key_value(uint8_t const & key) const {
    std::optional <uint16_t> res = valid(key);
    if ( res.has_value() ) {
        return (std::find(_pressed_keys.begin(), _pressed_keys.end(), res.value()) != _pressed_keys.end());
    }
    return false;
}

void keyboard::set_key(uint8_t const & key) {
    std::optional <uint16_t> res = valid(key);

    if (res.has_value()) {
        _pressed_keys.push_back(res.value());
    }
}

void keyboard::clear_key(uint8_t const & key) {
    std::optional <uint16_t> res = valid(key);

    if (res.has_value()) {
        _pressed_keys.erase(
                std::find(_pressed_keys.begin(), _pressed_keys.end(),res.value())
                );
    }
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

std::optional<uint16_t> keyboard::valid(char const & c) const {
    for (auto [character, decode] : _keyboard) {
        if (character == c)
            return decode;
    }
    return {};
}
