#pragma once

#include "GameComponent.h"

class TPPCharacterController;
class Checkpoint;
class CameraComponent;
class BoxColliderComponent;

class TPPPlayer : public GameComponent
{
public:
	TPPPlayer(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

	void SetLastCheckpoint(Checkpoint* checkpoint);
	void BackToLastCheckpoint();

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
	Ref<TPPCharacterController> m_CharacterController;
	Ref<CameraComponent> m_Camera;
	
	glm::vec3 m_LastCheckpointPosition;

	/* Inputs */
	glm::vec3 m_MoveDirection;
	glm::vec3 m_Rotation;

	/* Flags */
	bool m_IsRunning;
	bool m_IsJumping;
	bool m_CanJump;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};