#include "sound/speakers.h"

#include <iostream>

speakers::speakers() {
    if ( SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cout << "init failed" << std::endl;
    }

    _audio_device = SDL_OpenAudioDevice(nullptr,
                                        0,
                                        &_desiredSpec,
                                        &_obtainedSpec,
                                        SDL_AUDIO_ALLOW_FREQUENCY_CHANGE
                                        );

    if (_audio_device == 0) {
        std::cout << "Failed opening audio device" << std::endl;
    }
}

speakers::~speakers() {
    SDL_CloseAudioDevice(_audio_device);
}

void speakers::play() {
    int duration = 140;
    double freq = 293.665;

    SDL_PauseAudioDevice(_audio_device, 0);

    BeepObject bo{
        .freq = freq,
        .samplesLeft = duration * FREQUENCY / 1000
    };
    _beeps.push(bo);
}

void speakers::stop() const {
    SDL_PauseAudioDevice(_audio_device, 1);
}

void speakers::generateSamples(uint16_t* stream, int length) {
    int i = 0;
    while (i < length) {

        if (_beeps.empty()) {
            while (i < length) {
                stream[i] = 0;
                i++;
            }
            return;
        }
        auto bo = _beeps.front();

        int samplesToDo = std::min(i + bo.samplesLeft, length);
        bo.samplesLeft -= samplesToDo - i;

        int v = 0;
        while (i < samplesToDo) {
            stream[i] = AMPLITUDE * std::sin(v * 2 * M_PI / FREQUENCY);
            i++;
            v += bo.freq;
        }

        if (bo.samplesLeft == 0) {
            _beeps.pop();
        }
    }
}

void audio_callback(void* userdata, uint8_t* stream, int len) {
    auto stream_buffer = reinterpret_cast<uint16_t *>(stream);
    int length = len / 2;
    auto beeper = reinterpret_cast<speakers *>(userdata);

    beeper->generateSamples(stream_buffer, length);
}