#pragma once

#include "Scene/Component/Component.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"

class ColliderComponent : public Component {
public:
    ColliderComponent(Actor *owner);

protected:
    bool isTrigger;

    virtual bool CheckCollisions() = 0;

    virtual glm::vec3 ClosestPoint(glm::vec3 point) = 0;

    friend class SceneSerializer;
};