#pragma once

#include "Scene/Component/Component.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent(Actor* owner);

protected:
	virtual bool CheckCollisions() = 0;

	friend class SceneSerializer;
};