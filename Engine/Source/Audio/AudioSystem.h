#pragma once

#include <map>

#include "Patterns/Singleton.h"

#include "fmod.hpp"
#include "fmod_errors.h"

enum CHANNEL_GROUP {
    MASTER, MUSIC, SFX
};

struct Implementation {
    Implementation();

    ~Implementation();

    void Update();

    FMOD::System *m_System;

    int m_NextChannelId;

    typedef std::map<std::string, FMOD::Sound *> SoundMap;
    typedef std::map<int, FMOD::Channel *> ChannelMap;
    typedef std::map<CHANNEL_GROUP, FMOD::ChannelGroup *> ChannelGroupMap;
    typedef std::map<CHANNEL_GROUP, float> VolumeMap;

    SoundMap m_Sounds;
    ChannelMap m_Channels;
    ChannelGroupMap m_ChannelGroups;
    VolumeMap m_ChannelVolumes;
};

class AudioSystem : public Singleton<AudioSystem> {
public:
    static void Initialize();

    static void Update(float deltaTime);

    static void Shutdown();

    static int ErrorCheck(FMOD_RESULT result);

    void LoadSound(const std::string &soundName, bool b3d = false, bool bLooping = false);

    void UnLoadSound(const std::string &soundName);

    void Set3dListener(const glm::vec3 &position, const glm::vec3 &forward, const glm::vec3 &up);

    int PlaySound(const std::string &soundName, CHANNEL_GROUP channelGroup, float volume = 0.5f, bool isLooping = false,
                  bool is3d = false, const glm::vec3 &position = glm::vec3(0.0f));

    void StopChannel(int channelId);

    void StopAllChannels();

    void SetChannel3dPosition(int channelId, const glm::vec3 &position);

    void SetChannelVolume(int channelId, float volume);

    void SetChannelGroupVolume(CHANNEL_GROUP channelGroup, float volume);

    void SetChannelMode(int channelId, bool is3d, bool looping);

    bool IsPlaying(int channelId) const;

    static FMOD_VECTOR Vec3ToFmod(const glm::vec3 &vec3);
};