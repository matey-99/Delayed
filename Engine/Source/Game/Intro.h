#pragma once

#include "GameComponent.h"

class TransformComponent;
class ImageComponent;

class Intro : public GameComponent {

public:

    Intro(Actor *owner);

    void Start() override;

    void Update(float deltaTime) override;

private:

    void FadeIn(float deltaTime);
    void FadeOut();

private:

    bool run;

    TimerHandle m_Timer1;

    Ref<TransformComponent> m_Spaceship;
    Ref<ImageComponent> m_FadeUI;

#pragma region Serialization

    uint32_t m_SpaceshipID;
    uint32_t m_FadeUIID;

#pragma endregion

    friend class SceneSerializer;
};

