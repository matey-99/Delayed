#include "Obelisk.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/SaveManager.h"
#include "Scene/Scene.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/Particle/ParticleSystemComponent.h"
#include "Scene/Component/UI/ImageComponent.h"
#include "Time/Time.h"
#include "Time/TimerManager.h"
#include "Game/Player.h"

Obelisk::Obelisk(Actor* owner)
	: GameComponent(owner)
{
	m_Used = false;
	m_TimeToGetEffect = 3.0f;

	m_PostFXID = 0;
}

Obelisk::~Obelisk()
{
}

void Obelisk::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
		collider->OnTriggerEnterDelegate.Add(&Obelisk::OnTriggerEnter, this);

	m_ParticleSystem = m_Owner->GetTransform()->GetChildren()[0]->GetOwner()->GetComponent<ParticleSystemComponent>();
	if (!m_ParticleSystem)
		ENGINE_WARN("There is no particle system attached to Obelisk");

	m_PostFX = m_Owner->GetScene()->GetComponent<ImageComponent>(m_PostFXID);
}

void Obelisk::Update(float deltaTime)
{
	if (m_Used)
	{
		HandleParticles();
		HandlePostFX();
	}
}

void Obelisk::OnTriggerEnter(ColliderComponent* other)
{
	if (m_Used)
		return;

	if (auto player = other->GetOwner()->GetComponent<Player>())
	{
		m_PlayerTransform = Cast<TransformComponent>(other->GetOwner()->GetTransform());

		m_ParticleSystem->GetOwner()->SetEnabled(true);
		m_PostFX->GetOwner()->SetEnabled(true);

		m_Player = player;
		m_Player->SlowDown();

		Event e;
		e.Add(&Obelisk::GetEffect, this);
		m_EffectTimerHandle = TimerManager::GetInstance()->SetTimer(e, m_TimeToGetEffect, false);

		m_Used = true;
	}
}

const SaveData Obelisk::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "Used", m_Used });

	return data;
}

void Obelisk::Load(const SaveData& data)
{
	bool used = data.BoolFields.find("Used")->second;

	m_Used = used;
}

void Obelisk::HandleParticles()
{
	glm::vec3 initialPosition = m_ParticleSystem->GetOwner()->GetTransform()->GetWorldPosition();
	glm::vec3 targetPosition = m_PlayerTransform->GetWorldPosition();
	glm::vec3 direction = Math::Normalize(targetPosition - initialPosition);
	glm::vec3 velocity = direction * Math::Distance(initialPosition, targetPosition);
	glm::vec3 delta = glm::vec3(1.0f);

	m_ParticleSystem->SetMinParticleVelocity(velocity - delta);
	m_ParticleSystem->SetMaxParticleVelocity(velocity + delta);
	m_ParticleSystem->SetEndParticleVelocity(velocity);
}

void Obelisk::HandlePostFX()
{
	float elapsedTime = Time::GetInstance()->GetElapsedTime();
	m_PostFX->SetColor({ glm::sin(3.0f * elapsedTime) + 3.0f, 1.0f, 1.0f, 0.3f });
}

void Obelisk::GetEffect()
{
	m_Player->BackToNormal();
	
	m_ParticleSystem->SetEmissionRateOverTime(0.0f);
	m_PostFX->GetOwner()->SetEnabled(false);

	TimerManager::GetInstance()->ClearTimer(m_EffectTimerHandle);
}
