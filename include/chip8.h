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

static constexpr uint16_t PROGRAM_START_ADDR = 0x200;
static constexpr uint16_t FONT_START_ADDR = 0x050; //standard position
static constexpr uint16_t DISPLAY_START_ADDR = 0xF00;

static constexpr uint8_t FONT_SIZE = 80;
static constexpr uint8_t DISPLAY_WIDTH = 64;
static constexpr uint8_t DISPLAY_HEIGHT = 32;

class chip8 {
public:
    chip8(keyboard & keyboard);
    ~chip8() = default;

    void update ();
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


    // TODO move this variables to memory
    uint16_t _program_counter;
    uint8_t _registers [16] {};
    uint16_t _index_register {};
    std::stack <uint16_t> _stack {};
    uint8_t _delay {};
    uint8_t _sound {};
    uint32_t _pixels [DISPLAY_WIDTH * DISPLAY_HEIGHT] {};
    // ----


    memory _ram {4096};

    display _display {"CHIP8", DISPLAY_WIDTH, DISPLAY_HEIGHT};

    keyboard _keyboard {};

    bool _draw = false;
    uint8_t font [FONT_SIZE] = {
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
