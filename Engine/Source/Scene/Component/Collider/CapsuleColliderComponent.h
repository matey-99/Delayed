#pragma once

#include "ColliderComponent.h"

class CapsuleColliderComponent : public ColliderComponent {
public:
    CapsuleColliderComponent(Actor *owner);

    virtual void Start() override;

    virtual void Update(float deltaTime) override;

protected:
    virtual bool CheckCollisions() override;

    glm::vec3 ClosestPoint(glm::vec3 point) override;
};