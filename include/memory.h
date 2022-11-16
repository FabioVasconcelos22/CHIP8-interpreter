#pragma once
#ifndef CHIP8_INTERPRETER_MEMORY_H
#define CHIP8_INTERPRETER_MEMORY_H

#include <cstdint>
#include <vector>

class memory {
public:
    memory(int size);

    ~memory() = default;

    size_t write (uint8_t* buffer, size_t buffer_size, uint16_t start_position);

    template<typename _t>
    _t read (int position);

    inline int size () const {
        return _size;
    }

private:
    std::vector <uint8_t> _ram {};

    int _size;
};


#endif //CHIP8_INTERPRETER_MEMORY_H
