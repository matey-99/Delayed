#include "AudioSystem.h"

void AudioSystem::Init() {
    FMOD::System_Create(&m_System);
    m_System->init(128, FMOD_INIT_NORMAL, nullptr);
    m_NextChannelId = 0;
}

void AudioSystem::Update(float deltaTime) {
    std::vector<ChannelMap::iterator> stoppedChannels;
    for (auto it = m_Channels.begin(), itEnd = m_Channels.end(); it != itEnd; ++it) {
        bool isPlaying = false;
        it->second->isPlaying(&isPlaying);
        if (!isPlaying) {
            stoppedChannels.push_back(it);
        }
    }
    for (auto &it: stoppedChannels) {
        m_Channels.erase(it);
    }
    m_System->update();
}

void AudioSystem::Shutdown() {
    m_System->close();
}

void AudioSystem::LoadSound(const std::string &soundName, bool is3d, bool isLooping, bool isStream) {
    auto foundIt = m_Sounds.find(soundName);
    if (foundIt != m_Sounds.end())
        return;
    FMOD_MODE mode = FMOD_DEFAULT;
    mode |= is3d ? FMOD_3D : FMOD_2D;
    mode |= isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    mode |= isStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
    FMOD::Sound *pSound = nullptr;
    FMOD_RESULT result = m_System->createSound(soundName.c_str(), mode, nullptr, &pSound);
    if (result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    }
    if (pSound) {
        m_Sounds[soundName] = pSound;
    }
}

void AudioSystem::UnLoadSound(const std::string &soundName) {
    auto foundIt = m_Sounds.find(soundName);
    if (foundIt == m_Sounds.end())
        return;
    foundIt->second->release();
    m_Sounds.erase(foundIt);
}

int AudioSystem::PlaySound(const std::string &soundName, const glm::vec3 &position, float volume) {
    int channelId = m_NextChannelId++;
    auto foundIt = m_Sounds.find(soundName);
    if (foundIt == m_Sounds.end()) {
        LoadSound(soundName);
        foundIt = m_Sounds.find(soundName);
        if (foundIt == m_Sounds.end()) {
            return channelId;
        }
    }
    FMOD::Channel *pChannel = nullptr;
    m_System->playSound(foundIt->second, nullptr, true, &pChannel);
    if (pChannel) {
        FMOD_VECTOR pos = {position.x, position.y, position.z};
        pChannel->set3DAttributes(&pos, nullptr);
        pChannel->setVolume(volume);
        pChannel->setPaused(false);
        m_Channels[channelId] = pChannel;
    }
    return channelId;
}

int AudioSystem::RegisterSound(const AudioSystem::SoundDefinition &soundDefinition, bool load) {
    return 0;
}

void AudioSystem::UnregisterSound(int soundId) {

}