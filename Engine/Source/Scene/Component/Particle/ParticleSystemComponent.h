#pragma once

#include "Scene/Component/RenderComponent.h"
#include "Math/Math.h"

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
			DistanceFromCamera = 0.0f;
		}

		bool operator<(Particle& other)
		{
			return DistanceFromCamera > other.DistanceFromCamera;
		}

		glm::vec3 Position;

		glm::vec3 InitialVelocity;
		glm::vec3 CurrentVelocity;
		
		glm::vec4 Color;

		float InitialSize;
		float CurrentSize;

		float InitialLifeTime;
		float CurrentLifeTime;

		float DistanceFromCamera;
	};

	enum class EmitterShape
	{
		Box, Sphere
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

	void ChangeSprite(std::string path);

	void SetMaxParticles(uint32_t count);
	void SetEmitterShape(EmitterShape shape);

	inline glm::vec4 GetStartParticleColor() const { return m_StartParticleColor; }
	inline glm::vec4 GetEndParticleColor() const { return m_EndParticleColor; }

	inline void SetDuration(float duration) { m_Duration = duration; }
	inline void SetLooping(bool looping) { m_Looping = looping; }
	inline void SetEmissionRateOverTime(float rateOverTime) { m_EmissionRateOverTime = rateOverTime; }
	inline void SetMinParticleVelocity(glm::vec3 minParticleVelocity) { m_MinParticleVelocity = minParticleVelocity; }
	inline void SetMaxParticleVelocity(glm::vec3 maxParticleVelocity) { m_MaxParticleVelocity = maxParticleVelocity; }
	inline void SetEndParticleVelocity(glm::vec3 endParticleVelocity) { m_EndParticleVelocity = endParticleVelocity; }
	inline void SetStartParticleColor(const glm::vec4& startParticleColor) { m_StartParticleColor = startParticleColor; }
	inline void SetEndParticleColor(const glm::vec4& endParticleColor) { m_EndParticleColor = endParticleColor; }

private:
	int FindUnusedParticle();
	void EmitParticle(int index);

private:
	float m_Duration;
	bool m_Looping;
	Ref<Texture> m_Sprite;
	float m_EmissionRateOverTime;
	Ref<ParticleEmitterShape> m_EmitterShape;
	uint32_t m_MaxParticles;
	float m_MinParticleSize;
	float m_MaxParticleSize;
	float m_EndParticleSize;
	glm::vec3 m_MinParticleVelocity;
	glm::vec3 m_MaxParticleVelocity;
	glm::vec3 m_EndParticleVelocity;
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

	float m_EmissionRateFractional;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};