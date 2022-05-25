#include "Obelisk.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/SaveManager.h"
#include "Scene/Scene.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/Particle/ParticleSystemComponent.h"
#include "Scene/Component/UI/ImageComponent.h"
#include "Time/Time.h"

Obelisk::Obelisk(Actor* owner)
	: GameComponent(owner)
{
	m_IsPlayerInRange = false;

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
		WARN("There is no particle system attached to Obelisk");

	m_PostFX = m_Owner->GetScene()->GetComponent<ImageComponent>(m_PostFXID);
}

void Obelisk::Update(float deltaTime)
{
	if (m_IsPlayerInRange)
	{
		HandleParticles();
		HandlePostFX();
	}
}

void Obelisk::OnTriggerEnter(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Player>())
	{
		m_PlayerTransform = Cast<TransformComponent>(other->GetOwner()->GetTransform());

		m_ParticleSystem->GetOwner()->SetEnabled(true);
		m_PostFX->GetOwner()->SetEnabled(true);

		m_IsPlayerInRange = true;
	}
}

const SaveData Obelisk::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "Enabled", m_Owner->IsEnabled() });

	return data;
}

void Obelisk::Load(const SaveData& data)
{
	bool enabled = data.BoolFields.find("Enabled")->second;

	m_Owner->SetEnabled(enabled);
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
