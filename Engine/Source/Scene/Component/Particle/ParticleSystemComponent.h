#pragma once

#include "Scene/Component/RenderComponent.h"

class ParticleEmitterShape;

class ParticleSystemComponent : public RenderComponent
{
private:
	struct Particle
	{
		Particle()
		{
			Position = glm::vec3(0.0f);
			InitialVelocity = glm::vec3(0.0f);
			CurrentVelocity = glm::vec3(0.0f);
			Color = glm::vec4(0.0f);
			InitialSize = 0.0f;
			CurrentSize = 0.0f;
			InitialLifeTime = 0.0f;
			CurrentLifeTime = 0.0f;
		}

		glm::vec3 Position;

		glm::vec3 InitialVelocity;
		glm::vec3 CurrentVelocity;
		
		glm::vec4 Color;

		float InitialSize;
		float CurrentSize;

		float InitialLifeTime;
		float CurrentLifeTime;
	};

public:
	ParticleSystemComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PreRender() override;
	virtual void Render(Material::BlendMode blendMode) override;
	virtual void Destroy() override;

	void Initialize();
	void Reset();

	void SetParticlesCount(uint32_t count);
	void SetParticleLifeTime(float lifeTime);
	void SetRadius(float radius);
	void SetMinVelocity(glm::vec3 minVelocity);
	void SetMaxVelocity(glm::vec3 maxVelocity);

private:
	int FindUnusedParticle();
	void EmitParticle(int index);

private:
	float m_Duration;
	bool m_Looping;
	float m_EmissionRateOverTime;
	Ref<ParticleEmitterShape> m_EmitterShape;
	uint32_t m_MaxParticles;
	float m_MinParticleSize;
	float m_MaxParticleSize;
	float m_EndParticleSize;
	glm::vec3 m_MinParticleVelocity;
	glm::vec3 m_MaxParticleVelocity;
	float m_MinParticleLifeTime;
	float m_MaxParticleLifeTime;
	glm::vec4 m_StartParticleColor;
	glm::vec4 m_EndParticleColor;

	float m_SimulationTime;
	float m_EmissionTime;
	std::vector<Particle> m_Particles;
	int m_LastUsedParticleIndex;

	Ref<Shader> m_Shader;

	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_PositionVBO;
	uint32_t m_ColorVBO;

	std::vector<glm::vec4> m_ParticlesPositions;
	std::vector<glm::vec4> m_ParticlesColors;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};