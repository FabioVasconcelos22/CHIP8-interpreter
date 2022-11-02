#include "memory.h"

#include <iostream>

memory::memory(int size) :
    _size {size}
{
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

template<typename _t>
_t memory::read (int position) {
    std::cout << "dimension not supported" << std::endl;
}

template<>
uint8_t memory::read <uint8_t> (int position) {
    uint16_t instruction = _ram.at(position);
    return instruction;
}

template<>
uint16_t memory::read <uint16_t> (int position) {
    uint16_t instruction = _ram.at(position) << 8 | _ram.at(position + 1);
    return instruction;
}

template<>
uint32_t memory::read <uint32_t> (int position) {
    uint32_t instruction =  _ram.at(position)       << 24 |
                            _ram.at(position + 1)   << 16 |
                            _ram.at(position + 3)   << 8  |
                            _ram.at(position + 4);
    return instruction;
}