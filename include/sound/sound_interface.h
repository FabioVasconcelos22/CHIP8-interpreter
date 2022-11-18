#pragma once
#ifndef CHIP8_INTERPRETER_SOUND_INTERFACE_H
#define CHIP8_INTERPRETER_SOUND_INTERFACE_H

class sound_interface {
public:
    virtual void play() = 0;
    virtual void stop() const = 0;
};
#endif //CHIP8_INTERPRETER_SOUND_INTERFACE_H
