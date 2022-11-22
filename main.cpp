#include <iostream>
#include <thread>
#include <chrono>

#include "chip8.h"
#include "display/monitor.h"
#include "sound/speakers.h"
#include "inputs/keyboard.h"

int main (int argc, char **argv) {
    auto timestamp = std::chrono::system_clock::now ();
    auto cpu_timestamp = std::chrono::system_clock::now();
    auto timers_timestamp = std::chrono::system_clock::now();

    if (argc != 4) {
        std::cout << "Invalid program call." << std::endl;
        std::cout << "Usage: " << std::endl;
        std::cout << "./chip8 [file path between quotation marks] [shift quirk, 0-false/1-true] [load store, 0-false/1-true]" << std::endl;
        exit (0);
    }

    std::string rom = argv[1];

    struct quirks {
        const bool shift;
        bool load_store;
    } quirks {
            static_cast<bool>(argv[2]),
            static_cast<bool>(argv[3])
    };

    std::unique_ptr <inputs_interface> inputs = std::make_unique <keyboard>(
            chip8_constant::KEYS
            );

    std::unique_ptr <display_interface> display = std::make_unique <monitor>(
            "CHIP8",
            chip8_constant::DISPLAY_WIDTH,
            chip8_constant::DISPLAY_HEIGHT,
            10
            );

    std::unique_ptr <sound_interface> sound = std::make_unique<speakers>();

    chip8 cpu (inputs.get(), quirks.shift, quirks.load_store);

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
                    inputs->set_key(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    inputs->clear_key(event.key.keysym.sym);
                    break;
            }
        }

        cpu.update (*display, *sound);
    }
    return 0;
}