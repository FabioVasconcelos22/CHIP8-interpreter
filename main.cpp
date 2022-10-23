#include <iostream>
#include <chrono>
#include <thread>

#include "chip8.h"
#include "keyboard.h"

int main (int argc, char **argv) {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    if (argc != 4) {
        std::cout << "Invalid program call." << std::endl;
        std::cout << "Usage: " << std::endl;
        std::cout << "./chip8 [file path] [shift quirk, 0-false/1-true] [load store, 0-false/1-true]" << std::endl;
        exit (0);
    }

    std::string rom = argv[1];
    auto shift_quirk = std::stoi (argv[2]);
    auto load_store_quirk = std::stoi (argv[3]);


    auto cpu_frame_rate = 2ms; //500Hz
    auto timers_frame_rate = 16ms; //60Hz

    keyboard keyboard;
    speakers speakers;

    chip8 cpu (keyboard, shift_quirk, load_store_quirk);

    if (! cpu.load_rom(rom)) {
        std::cout << "Rom file not found" << std::endl;
        exit(0);
    }

    SDL_Event event;
    bool running = true;

    auto timestamp = system_clock::now ();

    auto cpu_timestamp = system_clock::now();
    auto timers_timestamp = system_clock::now();

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