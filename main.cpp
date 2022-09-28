#include <iostream>
#include <chrono>
#include <thread>

#include "chip8.h"
#include "keyboard.h"

int main () {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    auto expected_frame_rate = 16ms;

    keyboard keyboard;
    chip8 cpu (keyboard);

    if (! cpu.load_rom("roms/chip8-test-suite.ch8")) {
        std::cout << "Rom file not found" << std::endl;
        exit(0);
    }

    SDL_Event event;
    bool running = true;

    auto timestamp = system_clock::now ();

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

        cpu.update();

        if ((system_clock::now () - timestamp) >= expected_frame_rate) {
            if (cpu.delay > 0) {
                cpu.delay --;
            }

            if (cpu.sound > 0) {
                cpu.sound --;
            }

            timestamp = system_clock::now ();
        }
    }

    return 0;
}