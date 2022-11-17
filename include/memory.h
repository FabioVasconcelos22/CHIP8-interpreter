#pragma once
#ifndef CHIP8_INTERPRETER_MEMORY_H
#define CHIP8_INTERPRETER_MEMORY_H

#include <cstdint>
#include <vector>

class memory {
public:
    memory(int size) :
            _size {size}
    {
        _ram.resize(size);
    }

    ~memory() = default;

    template <typename _t>
    inline size_t write (_t buffer, size_t buffer_size, int start_position) {
        if (start_position + buffer_size > _ram.capacity()) {
            return -1;
        }

        std::copy( buffer, buffer + buffer_size, _ram.begin() + start_position);

        return buffer_size;
    }

    template<typename _t>
    inline _t read (int position) {
        _t buffer;
        std::copy (_ram.begin () + position,
                   _ram.begin() + position + sizeof (_t),
                   reinterpret_cast < uint8_t * >(&buffer));

        return swap_endian <_t> (buffer);
    }

    // https://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c
    template <typename _t>
    _t swap_endian(_t u) {
        static_assert (__CHAR_BIT__ == 8, "CHAR_BIT != 8");

        union {
            _t u;
            unsigned char u8[sizeof(_t)];
        } source, dest;

        source.u = u;

        for (size_t k = 0; k < sizeof(_t); k++)
            dest.u8[k] = source.u8[sizeof(_t) - k - 1];

        return dest.u;
    }
    // ----------

    [[nodiscard]] inline int size () const {
        return _size;
    }

private:
    std::vector <uint8_t> _ram {};

    int _size;
};

#endif //CHIP8_INTERPRETER_MEMORY_H
