#include "SphereColliderComponent.h"

SphereColliderComponent::SphereColliderComponent(Actor* owner)
	: ColliderComponent(owner)
{
}

void SphereColliderComponent::Start()
{
}

void SphereColliderComponent::Update(float deltaTime)
{
}

void SphereColliderComponent::Destroy()
{
}

bool SphereColliderComponent::CheckCollisions()
{
	return false;
}
