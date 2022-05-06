#include "CharacterController.h"

#include "Scene/Actor.h"
#include "Math/Math.h"
#include "Physics/Physics.h"
#include "Time/Time.h"

CharacterController::CharacterController(Actor* owner)
	: GameComponent(owner)
{
	m_Velocity = glm::vec3(0.0f);
	m_DistanceToGround = 2.25f;
	m_MoveSmoothness = 0.89f;
	m_RotateSmoothness = 0.89f;
	m_IsGrounded = false;

	m_JumpHeight = 0.5f;
	m_JumpMaxHeightTime = 1.0f;

	m_Gravity = 0.0f;
	m_InitialSpeed = 0.0f;

	m_IsJumping = false;
}

void CharacterController::FixedUpdate()
{
	Ray ray(m_Owner->GetTransform()->GetWorldPosition(), -Math::UpVector);
	m_IsGrounded = Physics::RayCast(ray, m_DistanceToGround, false, m_Owner);
}

void CharacterController::Move(glm::vec3 motion)
{
	m_Gravity = -2.0f * m_JumpHeight / (m_JumpMaxHeightTime * m_JumpMaxHeightTime);
	m_InitialSpeed = 2.0f * m_JumpHeight / m_JumpMaxHeightTime;

	if (m_IsGrounded && !m_IsJumping)
		m_Velocity.y = 0.0f;
	else if (!m_IsGrounded && m_Velocity.y < 0.0f)
		m_Velocity.y += m_Gravity * 4.0f * Time::GetInstance()->GetDeltaTime();
	else
		m_Velocity.y += m_Gravity * Time::GetInstance()->GetDeltaTime();

	m_IsJumping = false;

	m_Velocity = motion * (1.0f - m_MoveSmoothness) + m_Velocity * m_MoveSmoothness;

	glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
	newPosition += m_Velocity;
	m_Owner->GetTransform()->SetLocalPosition(newPosition);
}

void CharacterController::Jump()
{
	if (m_IsGrounded)
	{
		m_IsJumping = true;
		m_Velocity.y = m_InitialSpeed;
	}
}
