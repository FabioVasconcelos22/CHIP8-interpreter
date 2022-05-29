#include "display.h"
#include <SDL2/SDL.h>

display::display(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);
}

display::~display() {

}

void display::run_window() {

}