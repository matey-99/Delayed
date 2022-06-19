#include "PickableSpaceshipPart.h"

#include "Scene/Actor.h"
#include "Player.h"
#include "SaveManager.h"
#include "Scene/Scene.h"
#include "NotificationManager.h"
#include "Inventory.h"

PickableSpaceshipPart::PickableSpaceshipPart(Actor* owner)
	: Interactable(owner)
{
	m_SpaceshipPart = SpaceshipPartType::Part1;
}

PickableSpaceshipPart::~PickableSpaceshipPart()
{
}

void PickableSpaceshipPart::Start()
{
}

const SaveData PickableSpaceshipPart::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "Enabled", m_Owner->IsEnabled() });

	return data;
}

void PickableSpaceshipPart::Load(const SaveData& data)
{
	bool enabled = data.BoolFields.find("Enabled")->second;

	m_Owner->SetEnabled(enabled);
}

void PickableSpaceshipPart::Interact(Player* player)
{
	PickUp(player);
}

void PickableSpaceshipPart::PickUp(Player* player)
{
	if (auto inventory = player->GetOwner()->GetComponent<Inventory>())
	{
		inventory->AddItem(m_SpaceshipPart);

		std::string spaceshipPartName = "";
		switch (m_SpaceshipPart)
		{
		case SpaceshipPartType::Part1:
			spaceshipPartName = "Part 1";
			break;
		case SpaceshipPartType::Part2:
			spaceshipPartName = "Part 2";
			break;
		case SpaceshipPartType::Part3:
			spaceshipPartName = "Part 3";
			break;
		}

		NotificationManager::GetInstance()->Notify("You collected a new spaceship part: " + spaceshipPartName);
		m_Owner->SetEnabled(false);
	}
}
