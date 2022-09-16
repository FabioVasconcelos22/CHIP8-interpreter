#pragma once
#ifndef CHIP8_INTERPRETER_KEYBOARD_H
#define CHIP8_INTERPRETER_KEYBOARD_H

#include <cinttypes>

class keyboard {
public:
    bool get_key_value(uint8_t const & index) const;
    void set_key (uint8_t const & key);
    void clear_key (uint8_t const & key);
private:
    bool _keys [16] {false};

    uint8_t ascii_to_chip8_key (uint8_t const& key);
};

#endif //CHIP8_INTERPRETER_KEYBOARD_H
