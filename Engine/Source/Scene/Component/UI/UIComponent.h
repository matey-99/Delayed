#pragma once

#include "Scene/Component/Component.h"

class UIComponent : public Component
{
public:
	UIComponent(Actor* owner) : Component(owner) {};

	virtual void Render() = 0;
};