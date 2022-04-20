#pragma once

#include "map"

#include "Patterns/Singleton.h"

#include "fmod.hpp"
#include "fmod_errors.h"

struct Implementation {
    Implementation() = default;
    ~Implementation() = default;

    void Update(float deltaTime);

    FMOD::System* system;

    int nextChannelId;

    typedef std::map<std::string, FMOD::Sound*> SoundMap;
    typedef std::map<int, FMOD::Channel*> ChannelMap;

    SoundMap sounds;
    ChannelMap channels;
};

class AudioSystem : public Singleton<AudioSystem> {
public:
    void Init();
    void Update(float deltaTime);
    void Shutdown();

    struct SoundDefinition {
        std::string soundName;
        float defaultVolumedB;
        float minDistance;
        float maxDistance;
        bool is3d;
        bool isLooping;
        bool isStreaming;
    };

    int RegisterSound(const SoundDefinition& soundDefinition, bool load = true);
    void UnregisterSound(int soundId);
    void LoadSound(const std::string &soundName, bool b3d = true, bool looping = false, bool stream = false);
    void UnLoadSound(const std::string &soundName);
    void Set3dListener(const glm::vec3 &position, const glm::vec3 &forward, const glm::vec3 &up);
    int PlaySound(const std::string &soundName, const glm::vec3 &position = glm::vec3(0.0f), float volume = 0.1f);
    void StopChannel(int channelId, float fadeTimeSeconds = 0.0f);
    void PauseChannel(int channelId);
    void StopAllChannels();
    void SetChannel3dPosition(int channelId, const glm::vec3 &position);
    void SetChannelVolume(int channelId, float volume);
    bool IsPlaying (int channelId) const;

private:
    Ref<Implementation> m_Implementation;

};