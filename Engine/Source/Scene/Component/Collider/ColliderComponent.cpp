#include "ColliderComponent.h"

ColliderComponent::ColliderComponent(Actor *owner)
        : Component(owner) {
    m_IsTrigger = false;
}

void ColliderComponent::Destroy()
{
    OnTriggerEnterDelegate.Clear();
    OnTriggerStayDelegate.Clear();
    OnTriggerExitDelegate.Clear();
}
