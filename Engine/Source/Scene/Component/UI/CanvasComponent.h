#pragma once

#include "Scene/Component/Component.h"

class CanvasComponent : public Component
{
private:

public:
	CanvasComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;
};