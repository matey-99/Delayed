#include "TPPCharacterController.h"

#include "Scene/Actor.h"
#include "Math/Math.h"
#include "Physics/Physics.h"
#include "Time/Time.h"
#include "CharacterController.h"
#include "CameraController.h"

TPPCharacterController::TPPCharacterController(Actor* owner)
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

void TPPCharacterController::FixedUpdate()
{
	Ray ray(m_Owner->GetTransform()->GetWorldPosition(), -Math::UpVector);
	m_IsGrounded = Physics::RayCast(ray, m_DistanceToGround, false, m_Owner);
}

void TPPCharacterController::Move(glm::vec3 direction, const CharacterMovementParams& params, float deltaTime)
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

void TPPCharacterController::Rotate(Ref<CameraController> camera, glm::vec3 inputDirection, float deltaTime)
{
    glm::vec3 currentRotation = m_Owner->GetTransform()->GetLocalRotation();
    glm::vec3 targetRotation = camera->GetOwner()->GetTransform()->GetLocalRotation();
    glm::vec3 rotation = glm::vec3(0.0f);

    float turnSpeed = 10.0f;

    // TODO: Fix fast 360 rotation
    if (inputDirection.z == 1.0f)
        rotation += glm::vec3(0,180,0);
    if (inputDirection.x == 1.0f)
        rotation += glm::vec3(0,90,0);
    if (inputDirection.x == -1.0f)
        rotation += glm::vec3(0,-90,0);
    if (inputDirection.z == 1.0f && inputDirection.x == 1.0f)
        rotation += glm::vec3(0,-135,0);
    if (inputDirection.z == 1.0f && inputDirection.x == -1.0f)
        rotation += glm::vec3(0,135,0);
    if (inputDirection.z == -1.0f && inputDirection.x == 1.0f)
        rotation += glm::vec3(0,-45,0);
    if (inputDirection.z == -1.0f && inputDirection.x == -1.0f)
        rotation += glm::vec3(0,45,0);

    targetRotation = Math::Lerp(currentRotation, targetRotation + rotation, deltaTime * turnSpeed);


    m_Owner->GetTransform()->SetLocalRotation(targetRotation);
}

void TPPCharacterController::Jump()
{
	if (m_IsGrounded)
	{
		m_IsJumping = true;
		m_Velocity.y = m_InitialSpeed;
	}
}
