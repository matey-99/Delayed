#pragma once

#include "ColliderComponent.h"

class SphereColliderComponent : public ColliderComponent {
public:
    SphereColliderComponent(Actor *owner);

    virtual void Start() override;

    virtual void Update(float deltaTime) override;

    virtual void FixedUpdate() override;

    void UpdateBoundingSphere();

    virtual bool IsIntersect(const Ray& ray) override;
    virtual bool IsIntersect(const glm::vec3& point) override;

    inline BoundingSphere GetBoundingSphere() const { return m_BoundingSphere; }
    inline glm::vec3 GetCenter() const { return m_Center; }
    inline float GetSize() const { return m_Size; }

    void SetCenter(const glm::vec3& center);
    void SetSize(const float& size);

protected:
    virtual bool CheckCollisions() override;

    glm::vec3 ClosestPoint(glm::vec3 point) override;


private:
    BoundingSphere m_BoundingSphere;

    glm::vec3 m_Center;
    float m_Size;

    glm::vec3 m_OwnerLastPosition;

    friend class SceneSerializer;
    friend class ActorDetailsPanel;
};