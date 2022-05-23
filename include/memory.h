#pragma once
#ifndef CHIP8_INTERPRETER_MEMORY_H
#define CHIP8_INTERPRETER_MEMORY_H

#include <cstdint>
#include <vector>

class memory {
public:
    memory(int size);

    ~memory() = default;

    size_t write (uint16_t* buffer, size_t buffer_size, uint16_t start_position);

private:
    std::vector <uint16_t> _ram {};
};


#endif //CHIP8_INTERPRETER_MEMORY_H
