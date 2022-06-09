#pragma once

#include "GameComponent.h"

class CameraComponent;
class BoxColliderComponent;

struct CharacterMovementParams
{
	bool IsWalking = false;
	bool IsRunning = false;
	bool IsSlowedDown = false;
};

class CharacterController : public GameComponent
{
public:
	CharacterController(Actor* owner);

	virtual void FixedUpdate() override;

	void Move(glm::vec3 direction, const CharacterMovementParams& params, float deltaTime);
	void Rotate(Ref<CameraComponent> camera, glm::vec3 rotation, float deltaTime);
	void MoveHead(const CharacterMovementParams& params, Ref<CameraComponent> camera, float deltaTime);
	void Jump();
	void Dash();

	inline bool IsGrounded() const { return m_IsGrounded; }
	inline float GetStamina() const { return m_Stamina; }
	float GetMovementSpeed() { return glm::length(m_Velocity); }

	inline void SetHeadDefaultPosition(glm::vec3 position) { m_HeadDefaultPosition = position; }

private:
	glm::vec3 m_Velocity;
	float m_DistanceToGround;
	float m_MoveSmoothness;
	float m_RotateSmoothness;
	bool m_IsGrounded;

	float m_WalkSpeed;
	float m_RunSpeed;
	float m_SlowedDownSpeed;
	float m_RotateSpeed;
	float m_LookUpLimit;
	float m_JumpHeight;
	float m_JumpMaxHeightTime;
	float m_DashDistance;
	float m_MaxStamina;
	float m_StaminaRestorePerSecond;
	float m_StaminaUsagePerJump;
	float m_StaminaUsagePerRunSecond;
	float m_StaminaUsagePerDash;
	glm::vec3 m_HeadDefaultPosition;
	float m_WalkHeadBobSpeed;
	float m_WalkHeadBobAmount;

	float m_Stamina;
	float m_Gravity;
	float m_InitialSpeed;
	float m_HeadBobTimer;

	bool m_IsJumping;
	bool m_IsDoubleJumping;
	bool m_IsOutOfStamina;

#pragma region Serialization

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};