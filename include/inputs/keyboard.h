#pragma once
#ifndef CHIP8_INTERPRETER_KEYBOARD_H
#define CHIP8_INTERPRETER_KEYBOARD_H

#include <map>
#include <vector>
#include <optional>

#include "inputs_interface.h"

class keyboard : public inputs_interface {
public:
    keyboard (std::map <char, uint16_t> & key_map);

    bool get_key_value(uint8_t const & index) const override;

    void set_key (uint8_t const & key) override;

    void clear_key (uint8_t const & key) override;
private:
    std::map<char, uint16_t> _keyboard;
    std::vector <char> _pressed_keys;

    bool _keys [16] {false};

    uint8_t ascii_to_chip8_key (uint8_t const& key);

    std::optional<uint16_t> valid (char const & c) const;
};

#endif //CHIP8_INTERPRETER_KEYBOARD_H
