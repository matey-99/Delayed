#pragma once

#include "GameComponent.h"

class CameraComponent;
class BoxColliderComponent;
struct CharacterMovementParams;

class TPPCharacterController : public GameComponent
{
public:
	TPPCharacterController(Actor* owner);

	virtual void FixedUpdate() override;

	void Move(glm::vec3 direction, const CharacterMovementParams& params, float deltaTime);
	void Rotate(Ref<CameraComponent> camera, glm::vec3 rotation, float deltaTime);
	void Jump();

	inline bool IsGrounded() const { return m_IsGrounded; }

private:
	glm::vec3 m_Velocity;
	float m_DistanceToGround;
	float m_MoveSmoothness;
	float m_RotateSmoothness;
	bool m_IsGrounded;

	float m_WalkSpeed;
	float m_RunSpeed;
	float m_RotateSpeed;
	float m_LookUpLimit;
	float m_JumpHeight;
	float m_JumpMaxHeightTime;

	float m_Gravity;
	float m_InitialSpeed;

	bool m_IsJumping;

#pragma region Serialization

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};