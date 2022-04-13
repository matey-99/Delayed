#pragma once

#include "ColliderComponent.h"
#include "Math/BoundingBox.h"

class BoxColliderComponent : public ColliderComponent {
public:
    BoxColliderComponent(Actor *owner);

    virtual void Start() override;

    virtual void Update(float deltaTime) override;

    virtual void FixedUpdate() override;

    virtual void Destroy() override;

    void UpdateBoundingBox();

    virtual bool IsIntersect(const Ray& ray) override;
    virtual bool IsIntersect(const glm::vec3& point) override;

    inline BoundingBox GetBoundingBox() const { return m_BoundingBox; }

protected:
    virtual bool CheckCollisions() override;

    glm::vec3 ClosestPoint(glm::vec3 point) override;

private:
    float Distance2ToPoint(glm::vec3 point);

private:
    BoundingBox m_BoundingBox;

    glm::vec3 m_Center;
    glm::vec3 m_Size;

    glm::vec3 m_OwnerLastPosition;

    friend class SceneSerializer;
    friend class ActorDetailsPanel;
};