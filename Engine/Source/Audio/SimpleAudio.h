#pragma once

#include "fmod.hpp"

class SimpleAudio {

public:
    SimpleAudio(std::string path);

    void Play();

private:
    FMOD::System* pSystem = nullptr;
    FMOD::Sound* pSound = nullptr;
    FMOD::Channel* pChannel = nullptr;
};

