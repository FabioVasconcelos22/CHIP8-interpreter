#include <iostream>
#include <chrono>
#include <thread>

#include "chip8.h"
#include "keyboard.h"

int main () {
    uint8_t frame_rate = 60;
    unsigned int delta_time = 1 / (float) frame_rate * 1000;

    keyboard keyboard;
    chip8 cpu (keyboard);
    cpu.load_rom("roms/test_opcode.ch8");

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    keyboard.set_key_pressed(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    keyboard.clear_key();
                    break;
            }

        }
        cpu.update();
        std::this_thread::sleep_for(std::chrono::milliseconds (delta_time));
    }
    return 0;
}