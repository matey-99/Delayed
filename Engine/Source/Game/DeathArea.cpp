#include "DeathArea.h"

#include "Scene/Actor.h"
#include "Player.h"

DeathArea::DeathArea(Actor* owner)
	: GameComponent(owner)
{
}

DeathArea::~DeathArea()
{
}

void DeathArea::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
	{
		collider->OnTriggerEnterDelegate.Add(&DeathArea::OnTriggerEnter, this);
	}
}

void DeathArea::OnTriggerEnter(ColliderComponent* other)
{
	if (auto player = other->GetOwner()->GetComponent<Player>())
		player->BackToLastCheckpoint();
}
