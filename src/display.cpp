#include "display/monitor.h"

#include <iostream>

monitor::monitor(const char * window_name, int width, int height, int scale) :
    _width {width},
    _height {height},
    _scale {scale}
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL Init failed with error: " << SDL_GetError() << std::endl;
        return;
    }

     _window = SDL_CreateWindow(window_name,
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                _width * _scale,
                                _height * _scale,
                                0);
    if (_window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (_renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_STATIC, width, height);
    if (_texture == nullptr) {
        std::cout << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
}

monitor::~monitor() {
    SDL_DestroyTexture(_texture);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void monitor::draw(uint32_t const & pixels) {
    int result;

    result = SDL_UpdateTexture(_texture, nullptr, &pixels, _width * sizeof(uint32_t));
    if (result != 0) {
        std::cout << "Failed to update texture. SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    result = SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
    if (result != 0) {
        std::cout << "Failed to copy texture to render. SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_RenderPresent(_renderer);
}

void monitor::clear() {
    SDL_RenderClear(_renderer);
    SDL_RenderPresent(_renderer);
}