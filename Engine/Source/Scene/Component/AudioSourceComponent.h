#pragma once

#include "Component.h"

#include "Audio/AudioSystem.h"

class AudioSourceComponent : public Component {

public:
    AudioSourceComponent(Actor *owner);

    void Start() override;

    void Update(float deltaTime) override;

    void Destroy() override;

    void ChangeSound(std::string path);

    void PlaySound();

    void StopSound();

    bool IsPlaying();

    void SetVolume(float volume);

private:
    std::string m_Sound;
    CHANNEL_GROUP m_Channel;
    float m_Volume;
    bool m_3d;
    bool m_Looping;
    bool m_PlayOnStart;

    int m_ChannelId;

    Ref<AudioSystem> m_AudioSystem;

    void UpdateMode();

    friend class SceneSerializer;

    friend class ActorDetailsPanel;
};

