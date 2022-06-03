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
    m_DistanceToGround = 1.1f;
    m_MoveSmoothness = 0.89f;
    m_RotateSmoothness = 0.89f;
    m_IsGrounded = false;

    m_WalkSpeed = 9.0f;
    m_RunSpeed = 13.0f;
    m_SlowedDownSpeed = 3.0f;
    m_RotateSpeed = 8.0f;
    m_LookUpLimit = 80.0f;
    m_JumpHeight = 0.05f;
    m_JumpMaxHeightTime = 0.43f;
    m_DashDistance = 4.0f;
    m_MaxStamina = 100.0f;
    m_StaminaRestorePerSecond = 15.0f;
    m_StaminaUsagePerJump = 5.0f;
    m_StaminaUsagePerRunSecond = 2.5f;
    m_StaminaUsagePerDash = 5.0f;

    m_Stamina = m_MaxStamina;
    m_Gravity = 0.0f;
    m_InitialSpeed = 0.0f;

    m_IsJumping = false;
    m_IsDoubleJumping = false;
}

void TPPCharacterController::FixedUpdate()
{
	Ray ray(m_Owner->GetTransform()->GetWorldPosition(), -Math::UpVector);
	m_IsGrounded = Physics::RayCast(ray, m_DistanceToGround, false, m_Owner);
}

void TPPCharacterController::Move(glm::vec3 direction, const CharacterMovementParams& params, float deltaTime)
{
    bool running = m_Stamina > 0.0f && params.IsRunning;
    float movementSpeed = params.IsSlowedDown ? m_SlowedDownSpeed : (running ? m_RunSpeed : m_WalkSpeed);

	m_Gravity = -2.0f * m_JumpHeight / (m_JumpMaxHeightTime * m_JumpMaxHeightTime);
	m_InitialSpeed = 2.0f * m_JumpHeight / m_JumpMaxHeightTime;

    if (m_IsGrounded && !m_IsJumping)
        m_Velocity.y = 0.0f;
    else if (!m_IsGrounded && m_Velocity.y < 0.0f)
        m_Velocity.y += m_Gravity * 1.25f * deltaTime;
    else
        m_Velocity.y += m_Gravity * deltaTime;

    if (m_IsGrounded)
    {
        m_IsJumping = false;
        m_IsDoubleJumping = false;

        if (params.IsRunning)
            m_Stamina -= m_StaminaUsagePerRunSecond * deltaTime;
        else
            m_Stamina += m_StaminaRestorePerSecond * deltaTime;
    }

    m_Stamina = glm::clamp(m_Stamina, 0.0f, m_MaxStamina);

    glm::vec3 motion = direction * movementSpeed * deltaTime;
    m_Velocity.x = motion.x * (1.0f - m_MoveSmoothness) + m_Velocity.x * m_MoveSmoothness;
    m_Velocity.z = motion.z * (1.0f - m_MoveSmoothness) + m_Velocity.z * m_MoveSmoothness;

    glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
    newPosition += m_Velocity;
    m_Owner->GetTransform()->SetLocalPosition(newPosition);
}

void TPPCharacterController::Rotate(Ref<CameraController> camera, glm::vec3 inputDirection, float deltaTime)
{
    glm::quat currentRotation = glm::quat(radians(m_Owner->GetTransform()->GetLocalRotation()));
    glm::quat targetRotation = glm::quat(radians(camera->GetOwner()->GetTransform()->GetLocalRotation()));

    float rotation = 0.0f;

    float turnSpeed = 10.0f;

    if (inputDirection.z == 1.0f)
        rotation += 180.0f;
    if (inputDirection.x == 1.0f)
        rotation += 90.0f;
    if (inputDirection.x == -1.0f)
        rotation += -90.0f;
    if (inputDirection.z == 1.0f && inputDirection.x == 1.0f)
        rotation += -135.0f;
    if (inputDirection.z == 1.0f && inputDirection.x == -1.0f)
        rotation += 135.0f;
    if (inputDirection.z == -1.0f && inputDirection.x == 1.0f)
        rotation += -45.0f;
    if (inputDirection.z == -1.0f && inputDirection.x == -1.0f)
        rotation += 45.0f;

    targetRotation = glm::slerp(currentRotation, glm::rotate(targetRotation, glm::radians(rotation), glm::vec3(0, 1, 0)), deltaTime * turnSpeed);

    m_Owner->GetTransform()->SetLocalRotation(glm::degrees(glm::eulerAngles(targetRotation)));
}

void TPPCharacterController::Jump()
{
    if (m_IsGrounded)
    {
        m_IsJumping = true;
        m_Velocity.y = m_InitialSpeed;

        m_Stamina -= m_StaminaUsagePerJump;
    }
    else if (!m_IsDoubleJumping)
    {
        m_IsDoubleJumping = true;
        m_Velocity.y = m_InitialSpeed;

        m_Stamina -= m_StaminaUsagePerJump;
    }
}

void TPPCharacterController::Dash()
{
    m_Velocity += m_Owner->GetTransform()->GetForward() * m_DashDistance;
    m_Stamina -= m_StaminaUsagePerDash;
}
