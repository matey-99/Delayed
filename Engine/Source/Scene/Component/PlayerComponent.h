#pragma once

#include "Component.h"

class PlayerComponent : public Component
{
private:

public:
	PlayerComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

private:
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
};