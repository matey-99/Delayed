#include "Spaceship.h"

#include "Scene/Actor.h"
#include "Player.h"
#include "SaveManager.h"
#include "Scene/Scene.h"
#include "NotificationManager.h"
#include "TutorialManager.h"
#include "Inventory.h"

Spaceship::Spaceship(Actor* owner)
	: Interactable(owner)
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

void Spaceship::Interact(Player* player)
{
	if (auto inventory = player->GetOwner()->GetComponent<Inventory>())
	{
		std::vector<SpaceshipPartType> collectedParts = inventory->GetItems();
		if (collectedParts.empty())
		{
			NotificationManager::GetInstance()->Notify("You have no collected spaceship parts!");
			return;
		}

		Fix(collectedParts);
		inventory->Clear();
	}
}

void Spaceship::Fix(const std::vector<SpaceshipPartType>& parts)
{
	for (auto& type : parts)
	{
		for (auto& part : m_SpaceshipParts)
		{
			if (part->GetType() == type)
			{
				part->Fix();
				break;
			}
		}
	}
}
