#include "display.h"

#include <iostream>

display::display(const char * window_name, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL Init failed with error: " << SDL_GetError() << std::endl;
        return;
    }

     _window = SDL_CreateWindow(window_name,
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width, height, 0);
    if (_window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (_renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (_texture == nullptr) {
        std::cout << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
}

display::~display() {
    SDL_DestroyTexture(_texture);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void display::draw(void const* buffer, int pitch) {
    auto result = 0;
    result = SDL_UpdateTexture(_texture, nullptr, buffer, pitch);
    if (result != 0) {
        std::cout << "Failed to update texture. SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    result = SDL_RenderClear(_renderer);
    if (result != 0) {
        std::cout << "Failed to clear render. SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    result = SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
    if (result != 0) {
        std::cout << "Failed to copy texture to render. SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_RenderPresent(_renderer);
}

void display::clear() {
    auto result = 0;
    result = SDL_RenderClear(_renderer);
    if (result != 0) {
        std::cout << "Failed to clear render. SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_RenderPresent(_renderer);

}