#pragma once

#include "GameComponent.h"

class CharacterController;
class Checkpoint;
class CameraComponent;
class BoxColliderComponent;
class Trail;

class Player : public GameComponent
{
public:
	Player(Actor* owner);

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

	void Dash();
	void AllowDashing();

	void Teleport();
	void AllowTeleporting();

	void HandleSkillsCooldowns(float deltaTime);
	void HandleHUD();

	void AddMovementInput(glm::vec3 direction, float value);

private:
	/* References */
	Ref<CharacterController> m_CharacterController;
	Ref<CameraComponent> m_Camera;
	Ref<Actor> m_Ghost;
	Ref<Trail> m_Trail;
	Ref<Actor> m_StaminaBar;
	
	/* Parameters */
	float m_DashCooldown;
	float m_TeleportCooldown;
	float m_TeleportTime;

	/* Inputs */
	glm::vec3 m_MoveDirection;
	glm::vec3 m_Rotation;

	/* Flags */
	bool m_IsRunning;
	bool m_IsJumping;
	bool m_IsTeleporting;
	bool m_CanJump;
	bool m_CanDash;
	bool m_CanTeleport;

	/* Others */
	glm::vec3 m_LastCheckpointPosition;
	glm::vec3 m_StaminaBarDefaultScale;
	glm::vec3 m_TeleportDestinationPosition;
	float m_DashCooldownTimer;
	float m_TeleportCooldownTimer;

#pragma region Serialization

	uint64_t m_CameraID;
	uint64_t m_GhostID;
	uint64_t m_TrailID;
	uint64_t m_StaminaBarID;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};