#include "AudioSystem.h"

void AudioSystem::Init() {
    m_Implementation = CreateRef<Implementation>();
}

void AudioSystem::Update(float deltaTime) {
    m_Implementation->Update(deltaTime);
}

void AudioSystem::Shutdown() {
}

void AudioSystem::LoadSound(const std::string &soundName, bool is3d, bool isLooping, bool isStream) {
    auto foundIt = m_Implementation->sounds.find(soundName);
    if(foundIt != m_Implementation->sounds.end())
        return;
    FMOD_MODE mode = FMOD_DEFAULT;
    mode |= is3d ? FMOD_3D : FMOD_2D;
    mode |= isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    mode |= isStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
    FMOD::Sound* pSound = nullptr;
    m_Implementation->system->createSound(soundName.c_str(), mode, nullptr, &pSound);
    if(pSound)
    {
        m_Implementation->sounds[soundName] = pSound;
    }
}

void AudioSystem::UnLoadSound(const std::string &soundName) {
    auto foundIt = m_Implementation->sounds.find(soundName);
    if(foundIt == m_Implementation->sounds.end())
        return;
    foundIt->second->release();
    m_Implementation->sounds.erase(foundIt);
}

int AudioSystem::PlaySound(const std::string &soundName, const glm::vec3 &position, float volume) {
    int channelId = m_Implementation->nextChannelId++;
    auto foundIt = m_Implementation->sounds.find(soundName);
    if(foundIt == m_Implementation->sounds.end())
    {
        LoadSound(soundName);
        foundIt = m_Implementation->sounds.find(soundName);
        if(foundIt == m_Implementation->sounds.end())
        {
            return channelId;
        }
    }
    FMOD::Channel* pChannel = nullptr;
    m_Implementation->system->playSound(foundIt->second, nullptr, true, &pChannel);
    if(pChannel)
    {
        FMOD_VECTOR pos = {position.x, position.y, position.z};
        pChannel->set3DAttributes(&pos, nullptr);
        pChannel->setVolume(volume);
        pChannel->setPaused(false);
        m_Implementation->channels[channelId] = pChannel;
    }
    return channelId;
}

int AudioSystem::RegisterSound(const AudioSystem::SoundDefinition &soundDefinition, bool load) {
    return 0;
}

void AudioSystem::UnregisterSound(int soundId) {

}

void Implementation::Update(float deltaTime) {
    std::vector<ChannelMap::iterator> stoppedChannels;
    for (auto it = channels.begin(), itEnd = channels.end(); it != itEnd; ++it) {
        bool isPlaying = false;
        it->second->isPlaying(&isPlaying);
        if (!isPlaying) {
            stoppedChannels.push_back(it);
        }
    }
    for (auto &it : stoppedChannels) {
        channels.erase(it);
    }
    system->update();
}