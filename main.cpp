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
    cpu.load_rom("roms/Airplane.ch8");

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    keyboard.set_key(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    keyboard.clear_key(event.key.keysym.sym);
                    break;
            }
        }

        if (cpu.delay <= 0) {
            cpu.update();
        } else {
            cpu.delay --;
        }

        if (cpu.sound > 0) {
            cpu.sound --;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds (delta_time));
    }
    return 0;
}