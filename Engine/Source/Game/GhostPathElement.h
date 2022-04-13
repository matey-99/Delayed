#pragma once

#include "GameComponent.h"

class GhostPathElement : public GameComponent
{
public:
	GhostPathElement(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

private:
	float m_LifeTime;
	float m_LifeTimer;
};