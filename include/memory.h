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

    uint16_t read_2bytes (uint16_t position);

    bool read (uint8_t* buffer, size_t size, uint16_t start_position);

private:
    std::vector <uint8_t> _ram {};
};


#endif //CHIP8_INTERPRETER_MEMORY_H
