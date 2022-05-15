#include "Trail.h"

#include "Scene/Actor.h"
#include "Scene/Component/Particle/ParticleSystemComponent.h"

Trail::Trail(Actor* owner)
	: GameComponent(owner)
{
}

void Trail::Start()
{
	m_ParticleSystem = m_Owner->GetComponent<ParticleSystemComponent>();
	if (!m_ParticleSystem)
		WARN("No Particle System Component in Actor with Trail Game Component!");
}

void Trail::EnableTrailParticlesEmission(bool enabled)
{
	if (enabled)
		m_ParticleSystem->SetEmissionRateOverTime(50.0f);
	else
		m_ParticleSystem->SetEmissionRateOverTime(0.0f);
}
