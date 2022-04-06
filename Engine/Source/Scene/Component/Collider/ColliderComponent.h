#pragma once

#include "Scene/Component/Component.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"
#include "Patterns/Delegate.h"

class ColliderComponent : public Component {
public:
    ColliderComponent(Actor *owner);

public:
    DelegateOneParam<Ref<ColliderComponent>> OnTriggerEnterDelegate;
    DelegateOneParam<Ref<ColliderComponent>> OnTriggerExitDelegate;

protected:
    bool m_IsTrigger;
    bool m_Triggered;

    virtual bool CheckCollisions() = 0;

    virtual glm::vec3 ClosestPoint(glm::vec3 point) = 0;

    friend class SceneSerializer;
};