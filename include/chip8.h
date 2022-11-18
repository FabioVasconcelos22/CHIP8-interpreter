#pragma once
#ifndef CHIP8_INTERPRETER_CHIP8_H
#define CHIP8_INTERPRETER_CHIP8_H

#include <cstdio>
#include <cinttypes>
#include <stack>
#include <string>
#include <memory>
#include <chrono>


#include "memory.h"
#include "keyboard.h"
#include "sound/speakers.h"
#include "display/display_interface.h"

namespace chip8_constant {
    constexpr uint16_t PROGRAM_START_ADDR = 0x200;
    constexpr uint16_t FONT_START_ADDR = 0x050; //standard position

    constexpr uint8_t FONT_SIZE = 80;
    constexpr uint8_t DISPLAY_WIDTH = 64;
    constexpr uint8_t DISPLAY_HEIGHT = 32;
    constexpr uint16_t MEMORY_SIZE = 4096;

    using namespace std::chrono;
    constexpr auto CPU_FRAME_RATE = 2ms; //500Hz
    constexpr auto TIMERS_FRAME_RATE = 16ms; //60Hz
}

class chip8 {
public:
    chip8(keyboard & keyboard,
          bool shift_quirk,
          bool load_store_quirk);
    ~chip8() = default;

    void run_instruction ();
    void update (display_interface & display, sound_interface & speakers);
    bool load_rom (const std::string& rom_path);

private:
    void interpret_instruction (uint16_t const & inst);
    void interpret_0_group (uint16_t const & inst);
    void interpret_1_group (uint16_t const & inst);
    void interpret_2_group (uint16_t const & inst);
    void interpret_3_group (uint16_t const & inst);
    void interpret_4_group (uint16_t const & inst);
    void interpret_5_group (uint16_t const & inst);
    void interpret_6_group (uint16_t const & inst);
    void interpret_7_group (uint16_t const & inst);
    void interpret_8_group (uint16_t const & inst);
    void interpret_9_group (uint16_t const & inst);
    void interpret_A_group (uint16_t const & inst);
    void interpret_B_group (uint16_t const & inst);
    void interpret_C_group (uint16_t const & inst);
    void interpret_D_group (uint16_t const & inst);
    void interpret_E_group (uint16_t const & inst);
    void interpret_F_group (uint16_t const & inst);

    uint16_t _program_counter {chip8_constant::PROGRAM_START_ADDR};

    std::array <uint8_t, 16> (_registers) {};

    uint16_t _index_register {};

    std::stack <uint16_t> _stack {};

    std::array <uint32_t, chip8_constant::DISPLAY_WIDTH * chip8_constant::DISPLAY_HEIGHT> _pixels {};

    memory _ram {chip8_constant::MEMORY_SIZE};

    keyboard* _keyboard;

    bool _draw = false;

    uint8_t _delay;
    uint8_t _sound;

    bool _shift_quirk = false;

    bool _load_store_quirk = false;

    std::chrono::time_point <std::chrono::system_clock> _timestamp;
    std::chrono::time_point <std::chrono::system_clock> _cpu_timestamp;
    std::chrono::time_point <std::chrono::system_clock> _timers_timestamp;

    std::array <uint8_t, chip8_constant::FONT_SIZE> font = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    enum colors {
        ON_COLOR = 0xFFFFFFFF,
        OFF_COLOR = 0xFF000000
    };
};


#endif //CHIP8_INTERPRETER_CHIP8_H
