#pragma once

#include "GameComponent.h"

class ParticleSystemComponent;

class Trail : public GameComponent
{
public:
	Trail(Actor* owner);

	virtual void Start() override;

	void EnableTrailParticlesEmission(bool enabled);

private:
	Ref<ParticleSystemComponent> m_ParticleSystem;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};