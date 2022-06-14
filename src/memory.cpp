#include "memory.h"

#include <iostream>

memory::memory(int size) {
    _ram.resize(size);
}

size_t memory::write(uint8_t *buffer, size_t buffer_size, uint16_t start_position) {
    if (start_position + buffer_size > _ram.capacity()) {
        return -1;
    }
    _ram.erase(_ram.begin()+start_position, _ram.begin() + start_position + static_cast<int>(buffer_size));
    _ram.insert(_ram.begin()+start_position, buffer, buffer + buffer_size);

    return buffer_size;
}

uint16_t memory::read_2bytes(uint16_t position) {
    uint16_t instruction = _ram.at(position) << 8 | _ram.at(position + 1);
    return instruction;
}

bool memory::read(uint8_t * buffer, size_t size, uint16_t start_position) {

    if (start_position < 0) {
        return false;
    }

    if (start_position + size > _ram.size()) {
        return false;
    }

    int pos = 0;
    for ( int i = start_position; i < start_position + size; ++i) {
        buffer [pos] = _ram.at(i);
        pos++;
    }

    return true;
}
