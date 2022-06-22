#include "TutorialTrigger.h"

#include "Scene/Actor.h"
#include "Player.h"

TutorialTrigger::TutorialTrigger(Actor* owner)
	: GameComponent(owner)
{
	m_Type = TutorialType::Movement;
}

TutorialTrigger::~TutorialTrigger()
{
}

void TutorialTrigger::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
	{
		collider->OnTriggerEnterDelegate.Add(&TutorialTrigger::OnTriggerEnter, this);
	}
}

void TutorialTrigger::OnTriggerEnter(ColliderComponent* other)
{
	if (auto player = other->GetOwner()->GetComponent<Player>())
	{
		TutorialManager::GetInstance()->DisplayTutorial(m_Type);
		m_Owner->SetEnabled(false);
	}
}

const SaveData TutorialTrigger::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "Enabled", m_Owner->IsEnabled() });

	return data;
}

void TutorialTrigger::Load(const SaveData& data)
{
	bool enabled = data.BoolFields.find("Enabled")->second;

	m_Owner->SetEnabled(enabled);
}
