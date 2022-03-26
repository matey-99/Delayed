#pragma once

#include "Scene/Component/TransformComponent.h"

#include <glm/glm.hpp>

class RectTransformComponent : public TransformComponent
{
public:
	RectTransformComponent(Actor* owner);

	friend class SceneSerializer;
};