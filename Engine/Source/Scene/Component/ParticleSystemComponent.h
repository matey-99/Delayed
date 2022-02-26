#pragma once

#include "Scene/Component/RenderComponent.h"
#include "Renderer/ShaderStorageBuffer.h"
#include "Renderer/ComputeShader.h"

class ParticleSystemComponent : public RenderComponent
{
public:
	ParticleSystemComponent(Entity* owner);

	virtual void Begin() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Destroy() override;

	void Reset();

	void SetParticlesCount(uint32_t count);
	void SetParticleLifeTime(float lifeTime);
	void SetRadius(float radius);
	void SetMinVelocity(glm::vec3 minVelocity);
	void SetMaxVelocity(glm::vec3 maxVelocity);

private:
	Ref<ShaderStorageBuffer<glm::vec4>> m_PositionBuffer;
	Ref<ShaderStorageBuffer<glm::vec4>> m_VelocityBuffer;
	Ref<ShaderStorageBuffer<uint32_t>> m_IndexBuffer;

	Ref<ComputeShader> m_ComputeShader;

	uint32_t m_NoiseTexture;

	uint32_t m_ParticleVAO;

	uint32_t m_ParticlesCount;
	float m_Radius;
	glm::vec3 m_MinVelocity;
	glm::vec3 m_MaxVelocity;
	float m_ParticleLifeTime;

	float m_ParticlesLifeTimeCounter;

	friend class SceneSerializer;
	friend class EntityDetailsPanel;
};