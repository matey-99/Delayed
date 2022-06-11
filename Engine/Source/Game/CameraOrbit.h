#pragma once

#include "GameComponent.h"

class CameraComponent;
class TransformComponent;

class CameraOrbit : public GameComponent {
public:
    CameraOrbit(Actor *owner);

    void Start() override;
    void Update(float deltaTime) override;

private:
    float m_Speed;

#pragma region Serialization;

#pragma endregion

    friend class SceneSerializer;
    friend class ActorDetailsPanel;
};

