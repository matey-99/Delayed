#include "Trail.h"

#include "Scene/Actor.h"
#include "Scene/Component/Particle/ParticleSystemComponent.h"

Trail::Trail(Actor* owner)
	: GameComponent(owner)
{
	m_DefaultStartColor = glm::vec4(0.0f);
	m_DefaultEndColor = glm::vec4(0.0f);
}

void Trail::Start()
{
	m_ParticleSystem = m_Owner->GetComponent<ParticleSystemComponent>();
	if (!m_ParticleSystem)
		ENGINE_WARN("No Particle System Component in Actor with Trail Game Component!");

	m_DefaultStartColor = m_ParticleSystem->GetStartParticleColor();
	m_DefaultEndColor = m_ParticleSystem->GetEndParticleColor();
}

const SaveData Trail::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();

	return data;
}

void Trail::Load(const SaveData& data)
{
	SetDefaultTrailParticlesColor();
}

void Trail::EnableTrailParticlesEmission(bool enabled)
{
	if (enabled)
		m_ParticleSystem->SetEmissionRateOverTime(50.0f);
	else
		m_ParticleSystem->SetEmissionRateOverTime(0.0f);
}

void Trail::SetDefaultTrailParticlesColor()
{
	m_ParticleSystem->SetStartParticleColor(m_DefaultStartColor);
	m_ParticleSystem->SetEndParticleColor(m_DefaultEndColor);
}

void Trail::ChangeTrailParticlesColor(const glm::vec4& startColor, const glm::vec4& endColor)
{
	m_ParticleSystem->SetStartParticleColor(startColor);
	m_ParticleSystem->SetEndParticleColor(endColor);
}
