#pragma once

#include "GameComponent.h"

class CharacterController;
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
	void AllowJumping();

	void RunOn();
	void RunOff();

	void AddMovementInput(glm::vec3 direction, float value);

private:
	/* References */
	Ref<CharacterController> m_CharacterController;
	Ref<CameraComponent> m_Camera;

	/* Inputs */
	glm::vec3 m_MoveDirection;
	glm::vec3 m_Rotation;

	/* Flags */
	bool m_IsRunning;
	bool m_IsJumping;
	bool m_CanJump;

	/* Movement properties */
	float m_MovementSpeed;
	float m_WalkSpeed;
	float m_RunSpeed;

	/* Camera */
	float m_RotateSpeed;
	float m_LookUpLimit;

#pragma region Serialization

	uint64_t m_CameraID;

#pragma endregion

	friend class SceneSerializer;
};