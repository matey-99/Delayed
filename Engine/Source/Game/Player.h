#pragma once

#include "Scene/Component/Component.h"

class Player : public Component
{
public:
	Player(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate() override;
	virtual void Destroy() override;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	

	void AddMovementInput(glm::vec3 direction, float value);

private:
	glm::vec3 m_MoveDirection;
	float m_MovementSpeed;
};