#pragma once

#include "TransformBaseComponent.h"

class TransformComponent : public TransformBaseComponent
{
public:
	TransformComponent(Actor* owner);

	virtual void CalculateLocalModelMatrix() override;

	friend class SceneSerializer;
};