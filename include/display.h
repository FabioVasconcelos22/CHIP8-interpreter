#pragma once
#ifndef CHIP8_INTERPRETER_DISPLAY_H
#define CHIP8_INTERPRETER_DISPLAY_H

#include <SDL2/SDL.h>

class display {
public:
    display(const char * window_name, int width, int height);
    ~display();

    void draw(void const* buffer, int pitch);
    void clear();
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Texture* _texture;
};


#endif //CHIP8_INTERPRETER_DISPLAY_H
