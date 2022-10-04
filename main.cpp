#include <iostream>
#include <chrono>
#include <thread>

#include "chip8.h"
#include "keyboard.h"

int main (int argc, char **argv) {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    if (argc != 3) {
        std::cout << "Invalid program call." << std::endl;
        std::cout << "Usage: " << std::endl;
        std::cout << "./chip8 [file path] [frame rate]" << std::endl;
        exit (0);
    }
    std::string rom = argv[1];

    auto expected_frame_rate = 16ms * std::stoi(argv[2]) / 60;

    keyboard keyboard;
    chip8 cpu (keyboard);

    if (! cpu.load_rom(rom)) {
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

        if ((system_clock::now () - timestamp) >= expected_frame_rate) {
            cpu.update();

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