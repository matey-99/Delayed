#pragma once

#include "GameComponent.h"

class CameraComponent;
class BoxColliderComponent;

class Player : public GameComponent
{
public:
	Player(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);

	void Jump();

	bool IsGrounded();
	
	void AddMovementInput(glm::vec3 direction, float value);

private:
	Ref<CameraComponent> m_Camera;
	Ref<BoxColliderComponent> m_GroundChecker;
	glm::vec3 m_MoveDirection;
	glm::vec3 m_Rotation;
	bool m_IsGrounded;

	float m_MovementSpeed;
	float m_RotateSpeed;
	float m_LookUpLimit;
	float smooth;
	float t_MovementSpeed;

#pragma region Serialization

	uint64_t m_CameraID;

#pragma endregion

	friend class SceneSerializer;
};