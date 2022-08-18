#pragma once
#ifndef CHIP8_INTERPRETER_DISPLAY_H
#define CHIP8_INTERPRETER_DISPLAY_H

#include <SDL2/SDL.h>

enum colors {
    ON_COLOR = 0xFFFFFFFF,
    OFF_COLOR = 0xFF000000
};

class display {
public:
    display(const char * window_name, int width, int height);
    ~display();

    void draw(uint32_t pixels[64 * 32]);

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Texture* _texture;

    int _width;
    int _height;
};


#endif //CHIP8_INTERPRETER_DISPLAY_H
