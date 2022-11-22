#pragma once
#ifndef CHIP8_INTERPRETER_INPUTS_INTERFACE_H
#define CHIP8_INTERPRETER_INPUTS_INTERFACE_H

#include <cinttypes>

class inputs_interface {
public:
    ~inputs_interface() = default;
    virtual bool get_key_value(uint8_t const & index) const = 0;

    virtual void set_key (uint8_t const & key) = 0;

    virtual void clear_key (uint8_t const & key) = 0;
};
#endif //CHIP8_INTERPRETER_INPUTS_INTERFACE_H
