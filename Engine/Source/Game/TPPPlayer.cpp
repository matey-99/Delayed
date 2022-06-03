#include "TPPPlayer.h"

#include "Scene/Actor.h"
#include "Input/Input.h"
#include "Math/Math.h"
#include "Math/Ray.h"
#include "Time/Time.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/RigidBodyComponent.h"
#include "CharacterController.h"
#include "TPPCharacterController.h"
#include "Trail.h"
#include "Checkpoint.h"
#include "CameraController.h"
#include "PickableSkill.h"
#include "TutorialManager.h"

TPPPlayer::TPPPlayer(Actor* owner)
	: Player(owner)
{
}

void TPPPlayer::Start()
{
	auto input = Input::GetInstance();

	input->BindAxis("Player_MoveForward", &TPPPlayer::MoveForward, this);
	input->BindAxis("Player_MoveRight", &TPPPlayer::MoveRight, this);

	input->BindAction("Jump", InputEvent::Press, &TPPPlayer::Jump, this);
	input->BindAction("Jump", InputEvent::Release, &TPPPlayer::AllowJumping, this);

	input->BindAction("Run", InputEvent::Press, &TPPPlayer::RunOn, this);
	input->BindAction("Run", InputEvent::Release, &TPPPlayer::RunOff, this);

    input->BindAction("Dash", InputEvent::Press, &TPPPlayer::Dash, this);
    input->BindAction("Dash", InputEvent::Release, &TPPPlayer::AllowDashing, this);

    input->BindAction("Teleport", InputEvent::Press, &TPPPlayer::Teleport, this);
    input->BindAction("Teleport", InputEvent::Release, &TPPPlayer::AllowTeleporting, this);

    input->SetInputMode(InputMode::Player);

	m_CharacterController = m_Owner->AddComponent<TPPCharacterController>();
    m_CameraController = m_Owner->GetScene()->GetComponent<CameraController>(m_CameraControllerID);
    m_Ghost = m_Owner->GetScene()->FindActor(m_GhostID);
    m_Trail = m_Owner->GetScene()->GetComponent<Trail>(m_TrailID);
    m_StaminaBar = m_Owner->GetScene()->FindActor(m_StaminaBarID);

    m_LastCheckpointPosition = m_Owner->GetTransform()->GetWorldPosition();
    m_StaminaBarDefaultScale = m_StaminaBar->GetTransform()->GetLocalScale();

}

void TPPPlayer::Update(float deltaTime)
{
    if (m_IsTeleporting)
    {
        auto currentPosition = m_Owner->GetTransform()->GetWorldPosition();

        if (!Math::IsNearlyEqual(currentPosition, m_TeleportDestinationPosition, 0.1f))
        {
            auto newPosition = Math::Lerp(currentPosition, m_TeleportDestinationPosition, 1 / m_TeleportTime * deltaTime);
            m_Owner->GetTransform()->SetWorldPosition(newPosition);

            return;
        }
        else
        {
            m_IsTeleporting = false;
        }
    }

	if (Math::Magnitude(m_MoveDirection) > 0.0f){
        m_MoveDirection = Math::Normalize(m_MoveDirection);
        m_CharacterController->Rotate(m_CameraController, m_InputDirection, deltaTime);
    }

    glm::vec3 currentPosition = m_Owner->GetTransform()->GetWorldPosition();
    if (!Math::IsNearlyEqual(currentPosition, m_LastPosition, 0.01f))
        m_Trail->EnableTrailParticlesEmission(true);
    else
        m_Trail->EnableTrailParticlesEmission(false);

	CharacterMovementParams params;
    params.IsWalking = glm::abs(m_MoveDirection.x) > 0.1f || glm::abs(m_MoveDirection.z) > 0.1f;
    params.IsRunning = m_IsRunning;
    params.IsSlowedDown = m_IsSlowedDown;

	m_CharacterController->Move(m_MoveDirection, params, deltaTime);

    HandleSkillsCooldowns(deltaTime);
    HandleHUD();

    m_LastPosition = currentPosition;

    // Reset move direction & rotation
	m_MoveDirection = glm::vec3(0.0f);
    m_InputDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
}

void TPPPlayer::MoveForward(float value)
{
    m_InputDirection += glm::normalize(glm::vec3(0, 0, value));
	AddMovementInput(m_CameraController->GetOwner()->GetTransform()->GetForward(), -value);
}

void TPPPlayer::MoveRight(float value)
{
    m_InputDirection += glm::normalize(glm::vec3(value, 0, 0));
	AddMovementInput(m_CameraController->GetOwner()->GetTransform()->GetRight(), -value);
}

void TPPPlayer::Jump()
{
    if (m_CanJump)
    {
        bool isGrounded = m_CharacterController->IsGrounded();
        if (isGrounded || m_HasDoubleJumpSkill)
        {
            m_CharacterController->Jump();
            m_CanJump = false;
        }

        if (!isGrounded && m_HasDoubleJumpSkill)
        {
            auto tutorial = TutorialManager::GetInstance();
            if (tutorial->IsTutorialDisplayed(TutorialType::DoubleJump))
                tutorial->HideTutorial(TutorialType::DoubleJump);
        }
    }
}

void TPPPlayer::AllowJumping()
{
    m_CanJump = true;
}

void TPPPlayer::RunOn()
{
    m_IsRunning = true;
}

void TPPPlayer::RunOff()
{
    m_IsRunning = false;
}

void TPPPlayer::Dash()
{
    if (m_CanDash && m_HasDashSkill && m_DashCooldownTimer == 0.0f)
    {
        m_CharacterController->Dash();
        m_CanDash = false;
        m_DashCooldownTimer = m_DashCooldown;

        auto tutorial = TutorialManager::GetInstance();
        if (tutorial->IsTutorialDisplayed(TutorialType::Dash))
            tutorial->HideTutorial(TutorialType::Dash);
    }
}

void TPPPlayer::AllowDashing()
{
    m_CanDash = true;
}

void TPPPlayer::Teleport()
{
    if (m_CanTeleport && m_HasTeleportSkill && m_TeleportCooldownTimer == 0.0f)
    {
        m_IsTeleporting = true;
        m_CanTeleport = false;
        m_TeleportCooldownTimer = m_TeleportCooldown;
        m_TeleportDestinationPosition = m_Ghost->GetTransform()->GetWorldPosition();

        auto tutorial = TutorialManager::GetInstance();
        if (tutorial->IsTutorialDisplayed(TutorialType::Teleport))
            tutorial->HideTutorial(TutorialType::Teleport);
    }
}

void TPPPlayer::AllowTeleporting()
{
    m_CanTeleport = true;
}

void TPPPlayer::HandleHUD()
{
    auto newStaminaBarScale = m_StaminaBar->GetTransform()->GetLocalScale();
    newStaminaBarScale.x = m_StaminaBarDefaultScale.x * m_CharacterController->GetStamina() / 100.0f;
    m_StaminaBar->GetTransform()->SetLocalScale(newStaminaBarScale);
}
