#include "CapsuleColliderComponent.h"

CapsuleColliderComponent::CapsuleColliderComponent(Actor* owner)
	: ColliderComponent(owner)
{
}

void CapsuleColliderComponent::Start()
{
}

void CapsuleColliderComponent::Update(float deltaTime)
{
}

void CapsuleColliderComponent::Destroy()
{
}

bool CapsuleColliderComponent::CheckCollisions()
{
	return false;
}
