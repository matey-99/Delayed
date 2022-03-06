#pragma once

#include "ColliderComponent.h"
#include "Math/BoundingBox.h"

class SphereColliderComponent : public ColliderComponent
{
public:
	SphereColliderComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;


protected:
	virtual bool CheckCollisions() override;
};