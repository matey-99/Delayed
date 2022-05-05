#pragma once

#include "Component.h"

#include "Audio/AudioSystem.h"

class AudioSourceComponent : public Component {

public:
    AudioSourceComponent(Actor *owner);

    void Start() override;

    void Update(float deltaTime) override;

    void Destroy() override;

    void SetSound(const std::string& sound);

    void SetVolume(float volume);

    void Set3d(bool is3d);

    void SetLooping(bool isLooping);

    void PlaySound();

private:
    std::string m_Sound;
    float m_Volume = 1.0f;
    bool m_3d = false;
    bool m_Looping = false;

    int m_ChannelId;

    Ref<AudioSystem> m_AudioSystem;
};

