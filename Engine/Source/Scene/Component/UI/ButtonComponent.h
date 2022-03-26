#pragma once

#include "UIComponent.h"

class ButtonComponent : public UIComponent
{
private:

public:
	ButtonComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Destroy() override;
};