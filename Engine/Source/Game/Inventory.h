#pragma once

#include "GameComponent.h"
#include "SpaceshipPart.h"

class Inventory : public GameComponent
{
public:
	Inventory(Actor* owner);

	void AddItem(SpaceshipPartType item);
	void RemoveItem(SpaceshipPartType item);
	void Clear();
	bool IsItemCollected(SpaceshipPartType item);

	inline std::vector<SpaceshipPartType> GetItems() const { return m_Items; }

private:
	std::vector<SpaceshipPartType> m_Items;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};