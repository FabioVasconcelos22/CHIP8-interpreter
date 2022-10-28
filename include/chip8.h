#pragma once
#ifndef CHIP8_INTERPRETER_CHIP8_H
#define CHIP8_INTERPRETER_CHIP8_H

#include <cstdio>
#include <cinttypes>
#include <stack>
#include <string>
#include <memory>

#include "memory.h"
#include "display.h"
#include "keyboard.h"
#include "speakers.h"

static constexpr uint16_t PROGRAM_START_ADDR = 0x200;
static constexpr uint16_t FONT_START_ADDR = 0x050; //standard position
static constexpr uint16_t DISPLAY_START_ADDR = 0xF00;

static constexpr uint8_t FONT_SIZE = 80;
static constexpr uint8_t DISPLAY_WIDTH = 64;
static constexpr uint8_t DISPLAY_HEIGHT = 32;
static constexpr uint16_t MEMORY_SIZE = 4096;

class chip8 {
public:
    chip8(keyboard & keyboard, bool shift_quirk, bool load_store_quirk);
    ~chip8() = default;

    void update ();
    bool load_rom (const std::string& rom_path);

    uint8_t delay {};
    uint8_t sound {};
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

    uint16_t _program_counter {PROGRAM_START_ADDR};

    std::array <uint8_t, 16> (_registers) {};

    uint16_t _index_register {};

    std::stack <uint16_t> _stack {};

    std::array <uint32_t, DISPLAY_WIDTH * DISPLAY_HEIGHT> _pixels {};

    memory _ram {MEMORY_SIZE};

    display _display {"CHIP8", DISPLAY_WIDTH, DISPLAY_HEIGHT, 10};

    std::unique_ptr <keyboard> _keyboard;

    speakers _audio;

    bool _draw = false;

    bool _shift_quirk = false;

    bool _load_store_quirk = false;

    std::array <uint8_t, FONT_SIZE> (font) {
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
};


#endif //CHIP8_INTERPRETER_CHIP8_H
