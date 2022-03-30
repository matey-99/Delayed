#pragma once

#include "Component.h"

class PlayerComponent : public Component
{
public:
	PlayerComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	

	void AddMovementInput(glm::vec3 direction, float value);

private:
	glm::vec3 m_MoveDirection;
	float m_MovementSpeed;
};