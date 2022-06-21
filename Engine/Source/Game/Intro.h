#pragma once

#include "GameComponent.h"

class TransformComponent;
class ImageComponent;
class ParticleSystemComponent;

class Intro : public GameComponent {

public:

    Intro(Actor *owner);

    void Start() override;

    void Update(float deltaTime) override;

private:

    void FadeIn(float duration);
    void FadeOut(float duration);
    void Move(const Ref<TransformComponent>& transformComponent,float duration, glm::vec3 from, glm::vec3 to, bool next);
    void Wait(float duration);
    void ChangePhase();

private:

    float m_Time;
    int m_Phase;

    Ref<ImageComponent> m_FadeUI;

    Ref<TransformComponent> m_Spaceship;
    glm::vec3 m_SpaceshipPosition;

    Ref<TransformComponent> m_Meteor;
    glm::vec3 m_MeteorPosition;

    Ref<Actor> m_Planet;

    Ref<ParticleSystemComponent> m_Thruster;

#pragma region Serialization

    uint32_t m_SpaceshipID;
    uint32_t m_FadeUIID;
    uint32_t m_MeteorID;
    uint32_t m_PlanetID;
    uint32_t m_ThrusterID;

#pragma endregion

    friend class SceneSerializer;
};

