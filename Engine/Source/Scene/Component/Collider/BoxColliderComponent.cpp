#include <Scene/Component/RigidBodyComponent.h>
#include "BoxColliderComponent.h"

#include "Scene/Actor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Component/TransformComponent.h"
#include "SphereColliderComponent.h"

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
    if (auto staticMesh = m_Owner->GetComponent<StaticMeshComponent>()) {
        m_BoundingBox = BoundingBox(staticMesh->GetBoundingBox().Min * m_Size,
                                    staticMesh->GetBoundingBox().Max * m_Size);
    } else {
        auto temp = BoundingBox(-m_Size + m_Center, m_Size + m_Center);
        auto points = temp.GetPoints();

        for (auto &point: points)
            point = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(point, 1.0f);

        m_BoundingBox = BoundingBox(points);
    }
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

bool BoxColliderComponent::CheckCollisions() {
    auto scene = m_Owner->GetScene();
    auto actors = scene->GetActors();
    for (auto actor: actors) {
        if (actor.get() == m_Owner)
            continue;

        if (auto collider = actor->GetComponent<BoxColliderComponent>()) {
            if ((glm::abs(m_BoundingBox.Center.x - collider->GetBoundingBox().Center.x) <=
                 (m_BoundingBox.Extents.x + collider->GetBoundingBox().Extents.x)) &&
                (glm::abs(m_BoundingBox.Center.y - collider->GetBoundingBox().Center.y) <=
                 (m_BoundingBox.Extents.y + collider->GetBoundingBox().Extents.y)) &&
                (glm::abs(m_BoundingBox.Center.z - collider->GetBoundingBox().Center.z) <=
                 (m_BoundingBox.Extents.z + collider->GetBoundingBox().Extents.z))) {

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

                if (m_Owner->IsDynamic()) {
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
        }

        if (auto collider = actor->GetComponent<SphereColliderComponent>()) {
            if (Distance2ToPoint(collider->GetBoundingSphere().Center) <=
                collider->GetBoundingSphere().Radius * collider->GetBoundingSphere().Radius) {

                glm::vec3 d = ClosestPoint(collider->GetBoundingSphere().Center) - collider->GetBoundingSphere().Center;
                glm::vec3 v = glm::normalize(d) * (collider->GetBoundingSphere().Radius - glm::length(d));

                if (m_Owner->IsDynamic()) {
                    m_Owner->GetTransform()->SetWorldPosition(m_Owner->GetTransform()->GetWorldPosition() + v);
                }
            }
        }
    }

    return false;
}
