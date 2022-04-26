#pragma once

#include "fmod.hpp"

class SimpleAudio {

public:
    SimpleAudio(std::string path) {
        FMOD::System_Create(&pSystem);
        pSystem->init(128, FMOD_INIT_NORMAL, nullptr);
        pSystem->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &pSound);
    }

    void Play() {
        pSystem->playSound(pSound, nullptr, false, &pChannel);
        pChannel->setVolume(0.05f);
    }

private:
    FMOD::System* pSystem = nullptr;
    FMOD::Sound* pSound = nullptr;
    FMOD::Channel* pChannel = nullptr;
};

