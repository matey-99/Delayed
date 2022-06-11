#pragma once

#include "GameComponent.h"
#include "Saveable.h"

class ParticleSystemComponent;

class Trail : public GameComponent, public Saveable
{
public:
	Trail(Actor* owner);

	virtual void Start() override;

	virtual const SaveData Save() override;
	virtual void Load(const SaveData& data) override;

	void EnableTrailParticlesEmission(bool enabled);
	void SetDefaultTrailParticlesColor();
	void ChangeTrailParticlesColor(const glm::vec4& startColor, const glm::vec4& endColor);

private:
	Ref<ParticleSystemComponent> m_ParticleSystem;

	glm::vec4 m_DefaultStartColor;
	glm::vec4 m_DefaultEndColor;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};