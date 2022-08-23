#pragma once
#ifndef CHIP8_INTERPRETER_KEYBOARD_H
#define CHIP8_INTERPRETER_KEYBOARD_H

#include <cinttypes>

class keyboard {
public:
    uint8_t get_key_pressed () const;
    void set_key_pressed (uint8_t const & key);
    void clear_key ();
private:
    uint8_t _key = 0xFF;
};


#endif //CHIP8_INTERPRETER_KEYBOARD_H
