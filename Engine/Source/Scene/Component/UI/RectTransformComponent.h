#pragma once

#include "Scene/Component/TransformBaseComponent.h"

#include <glm/glm.hpp>

class RectTransformComponent : public TransformBaseComponent
{
public:
	RectTransformComponent(Actor* owner);

	virtual void CalculateLocalModelMatrix() override;

	friend class SceneSerializer;
};