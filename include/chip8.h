#pragma once
#ifndef CHIP8_INTERPRETER_CHIP8_H
#define CHIP8_INTERPRETER_CHIP8_H

#include <cstdio>
#include <cinttypes>
#include <stack>
#include <string>
#include <memory>

#include "memory.h"

class chip8 {
public:
    chip8();
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

    uint16_t program_counter;
    uint8_t index_register [16];
    uint16_t index_address {};
    std::stack <uint16_t> stack {};
    uint8_t delay {};
    uint8_t sound {};
    uint8_t variable [16] {};

    memory mem  {4096};

    static constexpr uint16_t START_PROGRAM_ADDRESS = 0x200;

    static constexpr uint8_t FONT_ADRR = 0x50;
    static constexpr short FONT_SIZE = 80;
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
