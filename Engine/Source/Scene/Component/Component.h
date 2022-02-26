#pragma once

#include <string>
#include <glm/glm.hpp>

#include "typedefs.h"

class Entity;

class Component
{
public:
	Component(Entity* owner) : m_Owner(owner) {}

	virtual void Begin() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;

	inline Entity* GetOwner() const { return m_Owner; }

protected:
	Entity* m_Owner;
};