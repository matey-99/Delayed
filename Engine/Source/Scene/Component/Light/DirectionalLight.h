#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(Actor* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer);
	~DirectionalLight();

	virtual void Use() override;
	virtual void SwitchOff() override;
	
	virtual void RenderShadowMap() override;

private:
	std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& projection, const glm::mat4& view);

	glm::mat4 CalculateLightSpaceMatrix(float nearClipPlane, float farClipPlane);

private:
	std::vector<float> m_ShadowCascadeLevels;
	std::vector<glm::mat4> m_LightSpaceMatrices;

	friend class ActorDetailsPanel;
};
