#include "AudioSystem.h"

Implementation::Implementation() {
    m_System = nullptr;
    m_NextChannelId = 0;
    AudioSystem::ErrorCheck(FMOD::System_Create(&m_System));
    AudioSystem::ErrorCheck(m_System->init(128, FMOD_INIT_NORMAL, nullptr));
}

Implementation::~Implementation() {
    AudioSystem::ErrorCheck(m_System->close());
}

void Implementation::Update() {
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
    AudioSystem::ErrorCheck(m_System->update());
}

Implementation *m_Implementation = nullptr;

void AudioSystem::Initialize() {
    m_Implementation = new Implementation;
}

void AudioSystem::Update(float deltaTime) {
    m_Implementation->Update();
}

void AudioSystem::LoadSound(const std::string &soundName, bool b3d, bool bLooping) {

    auto tFoundIt = m_Implementation->m_Sounds.find(soundName);
    if (tFoundIt != m_Implementation->m_Sounds.end())
        return;

    FMOD_MODE mode = FMOD_DEFAULT;
    mode |= b3d ? FMOD_3D : FMOD_2D;
    mode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

    FMOD::Sound *sound = nullptr;
    AudioSystem::ErrorCheck(m_Implementation->m_System->createSound(soundName.c_str(), mode, nullptr, &sound));
    if (sound) {
        m_Implementation->m_Sounds[soundName] = sound;
    }
}

void AudioSystem::UnLoadSound(const std::string &soundName) {
    auto tFoundIt = m_Implementation->m_Sounds.find(soundName);
    if (tFoundIt != m_Implementation->m_Sounds.end())
        return;

    AudioSystem::ErrorCheck(tFoundIt->second->release());
    m_Implementation->m_Sounds.erase(tFoundIt);
}

void AudioSystem::Set3dListener(const glm::vec3 &position, const glm::vec3 &forward, const glm::vec3 &up) {
    FMOD_VECTOR pos = Vec3ToFmod(position);
    FMOD_VECTOR fwd = Vec3ToFmod(forward);
    FMOD_VECTOR upv = Vec3ToFmod(up);
    AudioSystem::ErrorCheck(m_Implementation->m_System->set3DListenerAttributes(0, &pos, nullptr, &fwd, &upv));
}

int AudioSystem::PlaySound(const std::string &soundName, float volume, bool isLooping, bool is3d,
                           const glm::vec3 &position) {
    int channelId = m_Implementation->m_NextChannelId++;
    auto tFoundIt = m_Implementation->m_Sounds.find(soundName);

    if (tFoundIt == m_Implementation->m_Sounds.end()) {
        LoadSound(soundName, is3d, isLooping);
        tFoundIt = m_Implementation->m_Sounds.find(soundName);
        if (tFoundIt == m_Implementation->m_Sounds.end()) {
            return channelId;
        }
    }

    FMOD::Channel *pChannel = nullptr;
    AudioSystem::ErrorCheck(m_Implementation->m_System->playSound(tFoundIt->second, nullptr, true, &pChannel));

    if (pChannel) {
        FMOD_MODE currMode;
        tFoundIt->second->getMode(&currMode);
        if (currMode & FMOD_3D) {
            FMOD_VECTOR pos = Vec3ToFmod(position);
            AudioSystem::ErrorCheck(pChannel->set3DAttributes(&pos, nullptr));
        }
        AudioSystem::ErrorCheck(pChannel->setVolume(volume));
        AudioSystem::ErrorCheck(pChannel->setPaused(false));
        m_Implementation->m_Channels[channelId] = pChannel;
    }
    return channelId;
}

void AudioSystem::StopChannel(int channelId) {
    auto tFoundIt = m_Implementation->m_Channels.find(channelId);
    if (tFoundIt == m_Implementation->m_Channels.end())
        return;

    tFoundIt->second->stop();
    m_Implementation->m_Channels.erase(channelId);
}

void AudioSystem::StopAllChannels() {
    for (auto channel: m_Implementation->m_Channels) {
        channel.second->stop();
    }
}

void AudioSystem::SetChannel3dPosition(int channelId, const glm::vec3 &position) {
    auto tFoundIt = m_Implementation->m_Channels.find(channelId);
    if (tFoundIt == m_Implementation->m_Channels.end())
        return;

    FMOD_VECTOR pos = Vec3ToFmod(position);
    AudioSystem::ErrorCheck(tFoundIt->second->set3DAttributes(&pos, nullptr));
}

void AudioSystem::SetChannelVolume(int channelId, float volume) {
    auto tFoundIt = m_Implementation->m_Channels.find(channelId);
    if (tFoundIt == m_Implementation->m_Channels.end())
        return;

    AudioSystem::ErrorCheck(tFoundIt->second->setVolume(volume));
}

void AudioSystem::SetChannelMode(int channelId, bool is3d, bool looping) {
    auto tFoundIt = m_Implementation->m_Channels.find(channelId);
    if (tFoundIt == m_Implementation->m_Channels.end())
        return;

    FMOD_MODE mode = FMOD_DEFAULT;
    mode |= is3d ? FMOD_3D : FMOD_2D;
    mode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

    AudioSystem::ErrorCheck(tFoundIt->second->setMode(mode));
}

bool AudioSystem::IsPlaying(int channelId) const {
    auto tFoundIt = m_Implementation->m_Channels.find(channelId);
    if (tFoundIt == m_Implementation->m_Channels.end())
        return false;

    bool isPlaying = false;
    AudioSystem::ErrorCheck(tFoundIt->second->isPlaying(&isPlaying));
    return isPlaying;
}

FMOD_VECTOR AudioSystem::Vec3ToFmod(const glm::vec3 &vec3) {
    FMOD_VECTOR vec;
    vec.x = vec3.x;
    vec.y = vec3.y;
    vec.z = vec3.z;
    return vec;
}

int AudioSystem::ErrorCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        printf("FMOD ERROR: %s\n", FMOD_ErrorString(result));
        return 1;
    }
    return 0;
}

void AudioSystem::Shutdown() {
    delete m_Implementation;
}
