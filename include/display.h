#pragma once
#ifndef CHIP8_INTERPRETER_DISPLAY_H
#define CHIP8_INTERPRETER_DISPLAY_H


class display {
    display(int width, int height);
    ~display();
    void run_window ();
};


#endif //CHIP8_INTERPRETER_DISPLAY_H
