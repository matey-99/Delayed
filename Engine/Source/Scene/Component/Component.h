#pragma once

#include "Core.h"

class Actor;
class ColliderComponent;

class Component
{
public:
	Component(Actor* owner) : m_Owner(owner) {};

	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void FixedUpdate() {};
	virtual void Destroy() = 0;

	inline Actor* GetOwner() const { return m_Owner; }

protected:
	Actor* m_Owner;
};
