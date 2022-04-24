#pragma once

#include "GameComponent.h"

class CameraComponent;
class BoxColliderComponent;

class CharacterController : public GameComponent
{
public:
	CharacterController(Actor* owner);

	virtual void FixedUpdate() override;

	void Move(glm::vec3 motion);
	void Jump();

	inline bool IsGrounded() const { return m_IsGrounded; }

private:
	glm::vec3 m_Velocity;
	float m_DistanceToGround;
	float m_MoveSmoothness;
	float m_RotateSmoothness;
	bool m_IsGrounded;

	float m_JumpHeight;
	float m_JumpMaxHeightTime;

	float m_Gravity;
	float m_InitialSpeed;

	bool m_IsJumping;

#pragma region Serialization

#pragma endregion

	friend class SceneSerializer;
};