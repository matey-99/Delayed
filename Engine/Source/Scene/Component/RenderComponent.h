#pragma once

#include "Component.h"

#include "Renderer/Renderer.h"
#include "Material/Material.h"

class RenderComponent : public Component
{
public:
	RenderComponent(Actor* owner) : Component(owner) {};

	virtual void PreRender() = 0;
	virtual void Render(Material::BlendMode blendMode) = 0;
};