#pragma once
#ifndef CHIP8_INTERPRETER_SPEAKERS_H
#define CHIP8_INTERPRETER_SPEAKERS_H

#include <SDL2/SDL.h>
#include <queue>
#include "sound_interface.h"

constexpr int AMPLITUDE = 28000;
constexpr int FREQUENCY = 44100;

void audio_callback(void* userdata,
                    uint8_t* stream,
                    int len
);

struct BeepObject
{
    double freq;
    int samplesLeft;
};

class speakers : public sound_interface {
public:
    speakers();
    ~speakers();
    void play() override;
    void stop() const override;

    void generateSamples(uint16_t* stream, int length);


private:
    SDL_AudioDeviceID _audio_device;
    SDL_AudioSpec _desiredSpec {
            .freq = FREQUENCY,
            .format = AUDIO_S16SYS,
            .channels = 1,
            .samples = 2048,
            .callback = audio_callback,
            .userdata = this
    };

    SDL_AudioSpec _obtainedSpec;

    std::queue<BeepObject> _beeps;
};


#endif //CHIP8_INTERPRETER_SPEAKERS_H
