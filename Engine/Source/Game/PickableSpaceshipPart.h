#pragma once

#include "Core.h"

#include "Interactable.h"
#include "Saveable.h"
#include "SpaceshipPart.h"

class Spaceship;

class PickableSpaceshipPart : public Interactable, public Saveable
{
public:
	PickableSpaceshipPart(Actor* owner);
	~PickableSpaceshipPart();

	virtual void Start() override;

	virtual const SaveData Save() override;
	virtual void Load(const SaveData& data) override;

	virtual void Interact(Player* player) override;

	void PickUp(Player* player);

private:
	Ref<Spaceship> m_Spaceship;

	SpaceshipPartType m_SpaceshipPart;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};