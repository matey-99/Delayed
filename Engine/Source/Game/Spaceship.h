#pragma once

#include "Core.h"

#include "Interactable.h"
#include "Saveable.h"
#include "SpaceshipPart.h"

class Spaceship : public Interactable
{
public:
	Spaceship(Actor* owner);
	~Spaceship();

	virtual void Start() override;

	virtual void Interact(Player* player) override;

	void Fix(const std::vector<SpaceshipPartType>& parts);

private:
	std::vector<Ref<SpaceshipPart>> m_SpaceshipParts;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};