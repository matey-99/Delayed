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
		SaveManager::GetInstance()->SaveGame();
	}
}
