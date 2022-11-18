#pragma once
#ifndef CHIP8_INTERPRETER_MONITOR_H
#define CHIP8_INTERPRETER_MONITOR_H

#include <SDL2/SDL.h>
#include "display/display_interface.h"

class monitor : public display_interface {
public:
    monitor(const char * window_name, int width, int height, int scale);
    ~monitor();

    void draw(uint32_t const & pixels) override;
    void clear() override;

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Texture* _texture;

    int _width;
    int _height;
    int _scale;
};


#endif //CHIP8_INTERPRETER_MONITOR_H
