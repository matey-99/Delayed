#pragma once

#include "TransformBaseComponent.h"

class TransformComponent : public TransformBaseComponent
{
public:
	TransformComponent(Actor* owner);

	friend class SceneSerializer;
};