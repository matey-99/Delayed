#include "Spaceship.h"

#include "Scene/Actor.h"
#include "Player.h"
#include "SaveManager.h"
#include "Scene/Scene.h"
#include "NotificationManager.h"
#include "TutorialManager.h"
#include "Inventory.h"

Spaceship::Spaceship(Actor* owner)
	: GameComponent(owner)
{
}

Spaceship::~Spaceship()
{
}

void Spaceship::Start()
{
	for (auto& child : m_Owner->GetTransform()->GetChildren())
	{
		if (auto part = child->GetOwner()->GetComponent<SpaceshipPart>())
			m_SpaceshipParts.push_back(part);
	}
}

void Spaceship::Fix(SpaceshipPartType part)
{
	for (auto& spaceshipPart : m_SpaceshipParts)
	{
		if (spaceshipPart->GetType() == part)
		{
			spaceshipPart->Fix();
			break;
		}
	}
}
