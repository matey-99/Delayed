#pragma once

#include "Component.h"

#include "Renderer/Renderer.h"

class RenderComponent : public Component
{
public:
	RenderComponent(Actor* owner) : Component(owner) {};

	virtual void PreRender() = 0;
	virtual void Render() = 0;
};