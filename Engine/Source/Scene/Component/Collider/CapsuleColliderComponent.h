#pragma once

#include "ColliderComponent.h"

class CapsuleColliderComponent : public ColliderComponent
{
public:
	CapsuleColliderComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

protected:
	virtual bool CheckCollisions() override;
};