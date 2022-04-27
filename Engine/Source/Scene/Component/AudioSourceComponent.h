#pragma once

#include "Component.h"

class AudioSourceComponent : public Component {

public:
    AudioSourceComponent(Actor *owner);

    void Start() override;

    void Update(float deltaTime) override;

    void Destroy() override;

    void SetSound(const std::string& sound);
    void PlaySound();

private:
    std::string m_SoundName;

};

