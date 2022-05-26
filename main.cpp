#include <iostream>
#include <chrono>
#include <thread>

#include "chip8.h"

int main () {
    uint8_t frame_rate = 60;
    unsigned int delta_time = 1 / (float) frame_rate * 1000;

    chip8 cpu;
    cpu.load_rom("roms/IBM_Logo.ch8");

    for (;;) {
        cpu.update();
        std::this_thread::sleep_for(std::chrono::milliseconds (delta_time));
    }
    return 0;
}