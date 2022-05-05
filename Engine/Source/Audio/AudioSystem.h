#pragma once

#include <map>

#include "Patterns/Singleton.h"

#include "fmod.hpp"
#include "fmod_errors.h"

struct Implementation {
    Implementation();
    ~Implementation();

    void Update();

    FMOD::System* m_System;

    int m_NextChannelId;

    typedef std::map<std::string, FMOD::Sound*> SoundMap;
    typedef std::map<int, FMOD::Channel*> ChannelMap;

    SoundMap m_Sounds;
    ChannelMap m_Channels;
};

class AudioSystem : public Singleton<AudioSystem> {
public:
    static void Initialize();
    static void Update(float deltaTime);
    static void Shutdown();
    static int ErrorCheck(FMOD_RESULT result);

    void LoadSound(const std::string &soundName, bool b3d = false, bool bLooping = false, bool bStream = false);
    void UnLoadSound(const std::string &soundName);
    void Set3dListener(const glm::vec3 &position, const glm::vec3 &forward, const glm::vec3 &up);
    int PlaySound(const std::string &soundName, float volume = 0.5f, bool isLooping = false, bool is3d = false, const glm::vec3 &position = glm::vec3(0.0f));
    void StopChannel(int channelId);
    void StopAllChannels();
    void SetChannel3dPosition(int channelId, const glm::vec3 &position);
    void SetChannelVolume(int channelId, float volume);
    bool IsPlaying (int channelId) const;
    static FMOD_VECTOR Vec3ToFmod(const glm::vec3 &vec3);
};