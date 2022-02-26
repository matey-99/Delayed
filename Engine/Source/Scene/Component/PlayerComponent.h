#pragma once

#include "InGameComponent.h"

class PlayerComponent : public InGameComponent
{
private:

public:
	PlayerComponent(Entity* owner);

	virtual void Begin() override;
	virtual void Update() override;
	virtual void Destroy() override;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void EndPlay() override;

private:
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
	void RotateHeadLeft();
	void RotateHeadRight();
};