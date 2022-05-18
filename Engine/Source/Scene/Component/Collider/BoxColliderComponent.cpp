#include <Scene/Component/RigidBodyComponent.h>
#include "BoxColliderComponent.h"

#include "Scene/Actor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Component/TransformComponent.h"
#include "SphereColliderComponent.h"
#include "Math/Math.h"

BoxColliderComponent::BoxColliderComponent(Actor *owner)
        : ColliderComponent(owner) {
    m_Center = glm::vec3(0.0f);
    m_Size = glm::vec3(1.0f);
}

void BoxColliderComponent::Start() {
    m_Owner->GetTransform()->OnTransformChanged.Add(&BoxColliderComponent::UpdateBoundingBox, this);

    UpdateBoundingBox();

    m_OwnerLastPosition = m_Owner->GetTransform()->GetWorldPosition();
}

void BoxColliderComponent::Update(float deltaTime) {
}

void BoxColliderComponent::FixedUpdate()
{
    CheckCollisions();
}

void BoxColliderComponent::Destroy() {
}

void BoxColliderComponent::UpdateBoundingBox() {
    if (auto staticMesh = m_Owner->GetComponent<StaticMeshComponent>()) 
    {
        glm::vec3 min = staticMesh->GetBoundingBox().Min;
        glm::vec3 max = staticMesh->GetBoundingBox().Max;
        glm::vec3 center = staticMesh->GetBoundingBox().Center;

        glm::vec3 deltaMin = glm::abs(min - center);
        glm::vec3 deltaMax = glm::abs(max - center);

        deltaMin *= m_Size;
        deltaMax *= m_Size;

        glm::vec3 newMin = center - deltaMin;
        glm::vec3 newMax = center + deltaMax;

        m_BoundingBox = BoundingBox(newMin + m_Center,
                                    newMax + m_Center);
    } else {
        auto temp = BoundingBox(-m_Size + m_Center, m_Size + m_Center);
        auto points = temp.GetPoints();

        for (auto &point: points)
            point = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(point, 1.0f);

        m_BoundingBox = BoundingBox(points);
    }
}

bool BoxColliderComponent::IsIntersect(const Ray& ray)
{
    return m_BoundingBox.IsIntersect(ray);
}

bool BoxColliderComponent::IsIntersect(const glm::vec3& point)
{
    return m_BoundingBox.IsIntersect(point);
}

glm::vec3 BoxColliderComponent::ClosestPoint(glm::vec3 point) {
    glm::vec3 t;
    float v;
    for (int i = 0; i < 3; ++i) {
        v = point[i];
        v = glm::max(v, m_BoundingBox.Min[i]);
        v = glm::min(v, m_BoundingBox.Max[i]);
        t[i] = v;
    }

    return t;
}

float BoxColliderComponent::Distance2ToPoint(glm::vec3 point) {
    float dist2 = 0.0f;

    for (int i = 0; i < 3; ++i) {
        float v = point[i];
        if (v < m_BoundingBox.Min[i]) dist2 += (m_BoundingBox.Min[i] - v) * (m_BoundingBox.Min[i] - v);
        if (v > m_BoundingBox.Max[i]) dist2 += (v - m_BoundingBox.Max[i]) * (v - m_BoundingBox.Max[i]);
    }

    return dist2;
}

void BoxColliderComponent::SetCenter(const glm::vec3& center)
{
    m_Center = center;
    UpdateBoundingBox();
}

void BoxColliderComponent::SetSize(const glm::vec3& size)
{
    m_Size = size;
    UpdateBoundingBox();
}

bool BoxColliderComponent::CheckCollisions() 
{
    if (!m_Owner->IsDynamic())
        return false;

    auto scene = m_Owner->GetScene();
    auto actors = scene->GetEnabledActors();
    for (auto actor : actors) {
        if (actor == m_Owner)
            continue;

        if (auto collider = actor->GetComponent<BoxColliderComponent>()) {
            if ((glm::abs(m_BoundingBox.Center.x - collider->GetBoundingBox().Center.x) <=
                 (m_BoundingBox.Extents.x + collider->GetBoundingBox().Extents.x)) &&
                (glm::abs(m_BoundingBox.Center.y - collider->GetBoundingBox().Center.y) <=
                 (m_BoundingBox.Extents.y + collider->GetBoundingBox().Extents.y)) &&
                (glm::abs(m_BoundingBox.Center.z - collider->GetBoundingBox().Center.z) <=
                 (m_BoundingBox.Extents.z + collider->GetBoundingBox().Extents.z))) 
            {
                if (m_CollisionColliders.find(collider) == m_CollisionColliders.end())
                {
                    m_CollisionColliders.insert(collider);

                    if (m_IsTrigger)
                        OnTriggerEnterDelegate.Broadcast(collider.get());

                    if (collider->IsTrigger())
                        collider->OnTriggerEnterDelegate.Broadcast(this);
                }
                else
                {
                    if (m_IsTrigger)
                        OnTriggerStayDelegate.Broadcast(collider.get());

                    if (collider->IsTrigger())
                        collider->OnTriggerStayDelegate.Broadcast(this);

                }

                if (!m_IsTrigger && !collider->m_IsTrigger)
                {
                    float left = m_BoundingBox.Max.x - collider->GetBoundingBox().Min.x;
                    float right = collider->GetBoundingBox().Max.x - m_BoundingBox.Min.x;
                    float bottom = m_BoundingBox.Max.y - collider->GetBoundingBox().Min.y;
                    float top = collider->GetBoundingBox().Max.y - m_BoundingBox.Min.y;
                    float backward = m_BoundingBox.Max.z - collider->GetBoundingBox().Min.z;
                    float forward = collider->GetBoundingBox().Max.z - m_BoundingBox.Min.z;

                    glm::vec3 v;
                    v.x = left < right ? -left : right;
                    v.y = bottom < top ? -bottom : top;
                    v.z = backward < forward ? -backward : forward;

                    if (glm::abs(v.x) < glm::abs(v.y) && glm::abs(v.x) < glm::abs(v.z)) {
                        v.y = 0;
                        v.z = 0;
                    } else if (glm::abs(v.y) < glm::abs(v.x) && glm::abs(v.y) < glm::abs(v.z)) {
                        v.x = 0;
                        v.z = 0;
                    } else if (glm::abs(v.z) < glm::abs(v.x) && glm::abs(v.z) < glm::abs(v.y)) {
                        v.x = 0;
                        v.y = 0;
                    }

                    m_Owner->GetTransform()->SetWorldPosition(m_Owner->GetTransform()->GetWorldPosition() + v);
                }
            }
            else
            {
                if (m_CollisionColliders.find(collider) != m_CollisionColliders.end())
                {
                    m_CollisionColliders.erase(collider);

                    if (m_IsTrigger)
                        OnTriggerExitDelegate.Broadcast(collider.get());

                    if (collider->IsTrigger())
                        collider->OnTriggerExitDelegate.Broadcast(this);
                }
            }
        }

        if (auto collider = actor->GetComponent<SphereColliderComponent>()) {
            if (Distance2ToPoint(collider->GetBoundingSphere().Center) <=
                collider->GetBoundingSphere().Radius * collider->GetBoundingSphere().Radius) 
            {
                // TO DO: Test trigger with sphere collider
                if (m_CollisionColliders.find(collider) == m_CollisionColliders.end())
                {
                    m_CollisionColliders.insert(collider);

                    if (m_IsTrigger)
                        OnTriggerEnterDelegate.Broadcast(collider.get());

                    if (collider->IsTrigger())
                        collider->OnTriggerEnterDelegate.Broadcast(this);
                }
                else
                {
                    if (m_IsTrigger)
                        OnTriggerStayDelegate.Broadcast(collider.get());

                    if (collider->IsTrigger())
                        collider->OnTriggerStayDelegate.Broadcast(this);

                }


                if (!collider->IsTrigger()) 
                {
                    glm::vec3 d = ClosestPoint(collider->GetBoundingSphere().Center) - collider->GetBoundingSphere().Center;
                    glm::vec3 v = glm::normalize(d) * (collider->GetBoundingSphere().Radius - glm::length(d));

                    m_Owner->GetTransform()->SetWorldPosition(m_Owner->GetTransform()->GetWorldPosition() + v);
                }
            }
            else
            {
                if (m_CollisionColliders.find(collider) != m_CollisionColliders.end())
                {
                    m_CollisionColliders.erase(collider);

                    if (m_IsTrigger)
                        OnTriggerExitDelegate.Broadcast(collider.get());

                    if (collider->IsTrigger())
                        collider->OnTriggerExitDelegate.Broadcast(this);
                }
            }
        }
    }

    return false;
}
