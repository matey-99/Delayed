#pragma once

#include "Component.h"

class AudioListenerComponent : public Component {

public:
    AudioListenerComponent(Actor *owner);

    void Start() override;

    void Update(float deltaTime) override;

    void Destroy() override;

};

