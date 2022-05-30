#include "Checkpoint.h"

#include "Scene/Actor.h"
#include "Player.h"
#include "SaveManager.h"

Checkpoint::Checkpoint(Actor* owner)
	: GameComponent(owner)
{
}

Checkpoint::~Checkpoint()
{
}

void Checkpoint::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
	{
		collider->OnTriggerEnterDelegate.Add(&Checkpoint::OnTriggerEnter, this);
	}
}

void Checkpoint::OnTriggerEnter(ColliderComponent* other)
{
	if (auto player = other->GetOwner()->GetComponent<Player>())
	{
		player->SetLastCheckpoint(this);
		m_Owner->SetEnabled(false);

		SaveManager::GetInstance()->SaveGame();
	}
}

const SaveData Checkpoint::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "Enabled", m_Owner->IsEnabled() });

	return data;
}

void Checkpoint::Load(const SaveData& data)
{
	bool enabled = data.BoolFields.find("Enabled")->second;

	m_Owner->SetEnabled(enabled);
}
