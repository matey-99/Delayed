#include "Inventory.h"

#include "Scene/Actor.h"

Inventory::Inventory(Actor* owner)
	: GameComponent(owner)
{
}

void Inventory::AddItem(SpaceshipPartType item)
{
	if (IsItemCollected(item))
		return;

	m_Items.push_back(item);
}

void Inventory::RemoveItem(SpaceshipPartType item)
{
	m_Items.erase(std::remove_if(m_Items.begin(), m_Items.end(), [item](const SpaceshipPartType& x)
	{
		return x == item;
	}));
}

void Inventory::Clear()
{
	m_Items.clear();
}

bool Inventory::IsItemCollected(SpaceshipPartType item)
{
	for (auto& it : m_Items)
	{
		if (it == item)
			return true;
	}

	return false;
}
