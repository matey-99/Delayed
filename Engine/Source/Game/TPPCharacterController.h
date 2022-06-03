#pragma once

#include "GameComponent.h"

class CameraController;
class BoxColliderComponent;
struct CharacterMovementParams;

class TPPCharacterController : public GameComponent
{
public:
	TPPCharacterController(Actor* owner);

	void FixedUpdate() override;

	void Move(glm::vec3 direction, const CharacterMovementParams& params, float deltaTime);
	void Rotate(Ref<CameraController> camera, glm::vec3 inputDirection, float deltaTime);
	void Jump();
    void Dash();

	inline bool IsGrounded() const { return m_IsGrounded; }
    inline float GetStamina() const { return m_Stamina; }

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

    float m_Stamina;
    float m_Gravity;
    float m_InitialSpeed;

    bool m_IsJumping;
    bool m_IsDoubleJumping;
    bool m_IsOutOfStamina;

#pragma region Serialization

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};