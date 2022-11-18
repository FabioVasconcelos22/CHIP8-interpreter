#pragma once
#ifndef CHIP8_INTERPRETER_DISPLAY_INTERFACE_H
#define CHIP8_INTERPRETER_DISPLAY_INTERFACE_H

#include <cinttypes>

class display_interface {
public:
    virtual void draw(uint32_t const & pixels) = 0;
    virtual void clear() = 0;
};
#endif //CHIP8_INTERPRETER_DISPLAY_INTERFACE_H
