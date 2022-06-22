#include "SphereColliderComponent.h"

#include "Scene/Actor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Component/TransformComponent.h"

SphereColliderComponent::SphereColliderComponent(Actor *owner)
        : ColliderComponent(owner) {
    m_Center = glm::vec3(0.0f);
    m_Size = 1.0f;

    m_OwnerLastPosition = glm::vec3(0.0f);
}

void SphereColliderComponent::Start() {
    m_Owner->GetTransform()->OnTransformChanged.Add(&SphereColliderComponent::UpdateBoundingSphere, this);

    UpdateBoundingSphere();
}

void SphereColliderComponent::Update(float deltaTime) {
}

void SphereColliderComponent::FixedUpdate()
{
    CheckCollisions();
}

void SphereColliderComponent::UpdateBoundingSphere() {
    if (auto staticMesh = m_Owner->GetComponent<StaticMeshComponent>()) {
        m_BoundingSphere = BoundingSphere(staticMesh->GetBoundingSphere().Center + m_Center,
                                          staticMesh->GetBoundingSphere().Radius * m_Size);
    } else {
        m_BoundingSphere = BoundingSphere(m_Owner->GetTransform()->GetWorldPosition() + m_Center, m_Size);
    }
}

bool SphereColliderComponent::IsIntersect(const Ray& ray)
{
    return m_BoundingSphere.IsIntersect(ray);
}

bool SphereColliderComponent::IsIntersect(const glm::vec3& point)
{
    return m_BoundingSphere.IsIntersect(point);
}

glm::vec3 SphereColliderComponent::ClosestPoint(glm::vec3 point) {
    glm::vec3 t = point - m_BoundingSphere.Center;
    return m_BoundingSphere.Center + (glm::normalize(t) * m_BoundingSphere.Radius);
}

void SphereColliderComponent::SetCenter(const glm::vec3& center)
{
    m_Center = center;
    UpdateBoundingSphere();
}

void SphereColliderComponent::SetSize(const float& size)
{
    m_Size = size;
    UpdateBoundingSphere();
}

//FIXME jittery collisions
bool SphereColliderComponent::CheckCollisions() 
{
    if (!m_Owner->IsDynamic())
        return false;

    auto scene = m_Owner->GetScene();
    auto actors = scene->GetEnabledActors();
    for (auto actor : actors) {
        if (actor == m_Owner)
            continue;

        if (auto collider = actor->GetComponent<SphereColliderComponent>()) {
            glm::vec3 d = m_BoundingSphere.Center - collider->GetBoundingSphere().Center;
            if (glm::length(d) < (collider->GetBoundingSphere().Radius + m_BoundingSphere.Radius)) {
                glm::vec3 v = glm::normalize(d) *
                              (collider->GetBoundingSphere().Radius + m_BoundingSphere.Radius - glm::length(d));

                m_Owner->GetTransform()->SetWorldPosition(m_Owner->GetTransform()->GetWorldPosition() + v);
            }
        }
    }

    return false;
}
