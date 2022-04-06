#include "ColliderComponent.h"

ColliderComponent::ColliderComponent(Actor *owner)
        : Component(owner) {
    m_IsTrigger = false;
    m_Triggered = false;
}
