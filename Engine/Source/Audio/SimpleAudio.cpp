#include "SimpleAudio.h"

SimpleAudio::SimpleAudio(std::string path) {
    FMOD::System_Create(&pSystem);
    pSystem->init(128, FMOD_INIT_NORMAL, nullptr);
    pSystem->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &pSound);
}

void SimpleAudio::Play() {
    pSystem->playSound(pSound, nullptr, false, &pChannel);
    pChannel->setVolume(0.05f);
}
