#pragma once

#include "Component.h"

class InGameComponent : public Component
{
public:
	InGameComponent(Entity* owner) : Component(owner) {};

	virtual void BeginPlay() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void EndPlay() = 0;
};