#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Saveable.h"
#include "SpaceshipPart.h"

class Spaceship : public GameComponent
{
public:
	Spaceship(Actor* owner);
	~Spaceship();

	virtual void Start() override;

	void Fix(SpaceshipPartType parts);

private:
	std::vector<Ref<SpaceshipPart>> m_SpaceshipParts;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};