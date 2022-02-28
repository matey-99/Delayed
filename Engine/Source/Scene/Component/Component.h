#pragma once

#include <string>
#include <glm/glm.hpp>

#include "typedefs.h"

class Actor;

class Component
{
public:
	Component(Actor* owner) : m_Owner(owner) {}

	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Destroy() = 0;

	inline Actor* GetOwner() const { return m_Owner; }

protected:
	Actor* m_Owner;
};