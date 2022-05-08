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

	m_WalkSpeed = 15.0f;
	m_RunSpeed = 25.0f;
	m_RotateSpeed = 8.0f;
	m_LookUpLimit = 80.0f;
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

void CharacterController::Move(glm::vec3 direction, const CharacterMovementParams& params, float deltaTime)
{
	float movementSpeed = params.IsRunning ? m_RunSpeed : m_WalkSpeed;

	m_Gravity = -2.0f * m_JumpHeight / (m_JumpMaxHeightTime * m_JumpMaxHeightTime);
	m_InitialSpeed = 2.0f * m_JumpHeight / m_JumpMaxHeightTime;

	if (m_IsGrounded && !m_IsJumping)
		m_Velocity.y = 0.0f;
	else if (!m_IsGrounded && m_Velocity.y < 0.0f)
		m_Velocity.y += m_Gravity * 3.0f * Time::GetInstance()->GetDeltaTime();
	else
		m_Velocity.y += m_Gravity * Time::GetInstance()->GetDeltaTime();

	m_IsJumping = false;

	glm::vec3 motion = direction * movementSpeed * deltaTime;
	m_Velocity = motion * (1.0f - m_MoveSmoothness) + m_Velocity * m_MoveSmoothness;

	glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
	newPosition += m_Velocity;
	m_Owner->GetTransform()->SetLocalPosition(newPosition);
}

void CharacterController::Rotate(Ref<CameraComponent> camera, glm::vec3 rotation, float deltaTime)
{
	glm::vec3 newRotation = m_Owner->GetTransform()->GetLocalRotation();
	newRotation.y -= rotation.y * m_RotateSpeed * deltaTime;
	m_Owner->GetTransform()->SetLocalRotation(newRotation);

	auto cameraTransform = camera->GetOwner()->GetTransform();
	glm::vec3 newCameraRotation = cameraTransform->GetLocalRotation();
	newCameraRotation.x -= rotation.x * m_RotateSpeed * deltaTime;
	newCameraRotation.x = glm::clamp(newCameraRotation.x, -m_LookUpLimit, m_LookUpLimit);
	cameraTransform->SetLocalRotation(newCameraRotation);
}

void CharacterController::Jump()
{
	if (m_IsGrounded)
	{
		m_IsJumping = true;
		m_Velocity.y = m_InitialSpeed;
	}
}
