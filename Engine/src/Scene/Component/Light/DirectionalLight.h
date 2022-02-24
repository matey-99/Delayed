#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer);
	~DirectionalLight();

	virtual void Use() override;
	virtual void SwitchOff() override;
	
	virtual void RenderShadowMap() override;

	friend class EntityDetailsPanel;
};
