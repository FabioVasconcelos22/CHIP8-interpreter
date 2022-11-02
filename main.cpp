#include <iostream>
#include <chrono>
#include <thread>

#include "chip8.h"
#include "display.h"

namespace display_info {
    static constexpr uint8_t DISPLAY_WIDTH = 64;
    static constexpr uint8_t DISPLAY_HEIGHT = 32;
}

int main (int argc, char **argv) {

    using namespace std::chrono;
    auto cpu_frame_rate = 2ms; //500Hz
    auto timers_frame_rate = 16ms; //60Hz

    auto timestamp = system_clock::now ();
    auto cpu_timestamp = system_clock::now();
    auto timers_timestamp = system_clock::now();

    if (argc != 4) {
        std::cout << "Invalid program call." << std::endl;
        std::cout << "Usage: " << std::endl;
        std::cout << "./chip8 [file path] [shift quirk, 0-false/1-true] [load store, 0-false/1-true]" << std::endl;
        exit (0);
    }

    std::string rom = argv[1];

    const struct quirks {
        bool shift;
        bool load_store;
    } quirks {
            static_cast<bool>(argv[2]),
            static_cast<bool>(argv[3])
    };

    keyboard keyboard;
    speakers speakers;
    display display {"CHIP8", display_info::DISPLAY_WIDTH,
                     display_info::DISPLAY_HEIGHT, 10};


    chip8 cpu (keyboard, quirks.shift, quirks.load_store);

    if (!cpu.load_rom(rom)) {
        std::cout << "Rom file not found" << std::endl;
        exit(0);
    }

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

        if ((system_clock::now () - cpu_timestamp) >= cpu_frame_rate) {
            cpu_timestamp = system_clock::now ();
            if (cpu.draw()) {
                display.draw(*cpu.pixels());
            }
            cpu.update();
        }

        if ((system_clock::now () - timers_timestamp) >= timers_frame_rate) {
            if (cpu.delay > 0) {
                cpu.delay--;
            }

            if (cpu.sound > 0) {
                speakers.play();
                cpu.sound--;

                if (cpu.sound <= 0) {
                    speakers.stop();
                }
            }
            timers_timestamp = system_clock::now();
        }
    }
    return 0;
}