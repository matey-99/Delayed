#pragma once

#include "Core.h"
#include "GameComponent.h"
#include "Player.h"

class Interactable : public GameComponent
{
public:
	Interactable(Actor* owner) 
		: GameComponent(owner)
	{}

	virtual void Interact(Player* player) = 0;
};