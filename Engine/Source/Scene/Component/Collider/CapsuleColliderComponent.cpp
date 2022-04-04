#include "CapsuleColliderComponent.h"

CapsuleColliderComponent::CapsuleColliderComponent(Actor *owner)
        : ColliderComponent(owner) {
}

void CapsuleColliderComponent::Start() {
}

void CapsuleColliderComponent::Update(float deltaTime) {
}

void CapsuleColliderComponent::Destroy() {
}

glm::vec3 CapsuleColliderComponent::ClosestPoint(glm::vec3 point) {
    return glm::vec3();
}

bool CapsuleColliderComponent::CheckCollisions() {
    return false;
}
