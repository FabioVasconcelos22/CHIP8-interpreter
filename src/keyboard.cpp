#include "keyboard.h"

uint8_t keyboard::get_key_pressed() const {
    return _key;
}

void keyboard::set_key_pressed(uint8_t const & key) {
    switch (key) {
        case '1':
            _key = 0x01;
            break;
        case '2':
            _key = 0x02;
            break;
        case '3':
            _key = 0x03;
            break;
        case 'q':
            _key = 0x04;
            break;
        case 'w':
            _key = 0x05;
            break;
        case 'e':
            _key = 0x06;
            break;
        case 'a':
            _key = 0x07;
            break;
        case 's':
            _key = 0x08;
            break;
        case 'd':
            _key = 0x09;
            break;
        case 'z':
            _key = 0x0A;
            break;
        case 'x':
            _key = 0x00;
            break;
        case 'c':
            _key = 0x0B;
            break;
        case 'r':
            _key = 0x0C;
            break;
        case 'f':
            _key = 0x0D;
            break;
        case 'v':
            _key = 0x0F;
            break;
        default:
            break;
    }
}

void keyboard::clear_key() {
    _key = 0xFF;
}