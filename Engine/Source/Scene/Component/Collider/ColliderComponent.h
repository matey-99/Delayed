#pragma once

#include "Scene/Component/Component.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"
#include "Patterns/Delegate.h"
#include "Core.h"

class ColliderComponent : public Component {
public:
    ColliderComponent(Actor *owner);

public:
    DelegateOneParam<ColliderComponent*> OnTriggerEnterDelegate;
    DelegateOneParam<ColliderComponent*> OnTriggerStayDelegate;
    DelegateOneParam<ColliderComponent*> OnTriggerExitDelegate;

    virtual bool IsIntersect(const Ray& ray) = 0;
    virtual bool IsIntersect(const glm::vec3& point) = 0;

    inline bool IsTrigger() const { return m_IsTrigger; }
    inline void SetTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }

protected:
    bool m_IsTrigger;

    /* All colliders that take a part in collision with this collider */
    std::unordered_set<Ref<ColliderComponent>> m_CollisionColliders;

    virtual bool CheckCollisions() = 0;

    virtual glm::vec3 ClosestPoint(glm::vec3 point) = 0;


    friend class SceneSerializer;
};