#include <Physics/Physics.h>
#include <Physics/RayCastHit.h>
#include "TPPPlayer.h"

#include "Scene/Actor.h"
#include "Input/Input.h"
#include "Math/Math.h"
#include "Math/Ray.h"
#include "Scene/Component/RigidBodyComponent.h"
#include "CharacterController.h"
#include "TPPCharacterController.h"
#include "Trail.h"
#include "CameraController.h"
#include "PickableSkill.h"
#include "TutorialManager.h"
#include "Inventory.h"
#include "InteractionPanel.h"
#include "GameManager.h"
#include "Interactable.h"
#include "Scene/Component/Animation/Animator.h"
#include "Ghost.h"

TPPPlayer::TPPPlayer(Actor* owner)
	: Player(owner)
{
    m_LastFrameIsGrounded = false;
}

void TPPPlayer::Start()
{
	auto input = Input::GetInstance();

	input->BindAxis("Player_MoveForward", &TPPPlayer::MoveForward, this);
	input->BindAxis("Player_MoveRight", &TPPPlayer::MoveRight, this);

	input->BindAction("Jump", InputEvent::Press, &TPPPlayer::Jump, this);
	input->BindAction("Jump", InputEvent::Release, &TPPPlayer::AllowJumping, this);

    input->BindAction("Jump_Gamepad", InputEvent::Press, &TPPPlayer::Jump_Gamepad, this);
    input->BindAction("Jump_Gamepad", InputEvent::Release, &TPPPlayer::AllowJumping_Gamepad, this);

	input->BindAction("Run", InputEvent::Press, &TPPPlayer::RunOn, this);
	input->BindAction("Run", InputEvent::Release, &TPPPlayer::RunOff, this);

    input->BindAction("Dash", InputEvent::Press, &TPPPlayer::Dash, this);
    input->BindAction("Dash", InputEvent::Release, &TPPPlayer::AllowDashing, this);

    input->BindAction("Teleport", InputEvent::Press, &TPPPlayer::Teleport, this);
    input->BindAction("Teleport", InputEvent::Release, &TPPPlayer::AllowTeleporting, this);

    input->BindAction("Interact", InputEvent::Press, &TPPPlayer::Interact, this);
    input->BindAction("Interact", InputEvent::Release, &TPPPlayer::AllowInteracting, this);

    input->SetInputMode(InputMode::Player);

	m_CharacterController = m_Owner->AddComponent<TPPCharacterController>();
    m_Inventory = m_Owner->AddComponent<Inventory>();
    m_CameraController = m_Owner->GetScene()->GetComponent<CameraController>(m_CameraControllerID);
    m_Ghost = m_Owner->GetScene()->GetComponent<Ghost>(m_GhostID);
    m_Trail = m_Owner->GetScene()->GetComponent<Trail>(m_TrailID);
    m_StaminaBar = m_Owner->GetScene()->FindActor(m_StaminaBarID);
    m_InteractionPanel = m_Owner->GetScene()->GetComponent<InteractionPanel>(m_InteractionPanelID);

    m_LastCheckpointPosition = m_Owner->GetTransform()->GetWorldPosition();
    m_StaminaBarDefaultScale = m_StaminaBar->GetTransform()->GetLocalScale();

    m_CharacterAnimator = m_Owner->GetTransform()->GetChildren()[0]->GetOwner()->GetComponent<Animator>();

    m_LastFrameIsGrounded = m_CharacterController->IsGrounded();
}

void TPPPlayer::Update(float deltaTime)
{
    m_IsGhostJumping = false;
    m_IsGhostDashing = false;

    if (GameManager::GetInstance()->IsGamePaused())
    {
        m_Interactable = nullptr;
        HideInteractionPanel();
    }
    else
        LookForInteractable();

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

	if (Math::Magnitude(m_MoveDirection) > 0.0f)
    {
        m_CharacterController->Rotate(m_CameraController, m_InputDirection, deltaTime);
        m_MoveDirection = Math::Normalize(m_MoveDirection);
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
    HandleAnimator();

    m_IsGhostJumping = m_IsJumping;
    m_IsGhostDashing = m_IsDashing;

    if (GetMovementSpeed() <= 0.8f)
        m_IsDashing = false;
    m_IsJumping = false;

    m_LastPosition = currentPosition;

    // Reset move direction & rotation
	m_MoveDirection = glm::vec3(0.0f);
    m_InputDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
}

float TPPPlayer::GetMovementSpeed()
{
    return m_CharacterController->GetMovementSpeed();
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
            m_IsJumping = true;
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

void TPPPlayer::Jump_Gamepad()
{
    if (m_CanJump_Gamepad)
    {
        bool isGrounded = m_CharacterController->IsGrounded();
        if (isGrounded || m_HasDoubleJumpSkill)
        {
            m_CharacterController->Jump();
            m_CanJump_Gamepad = false;
            m_IsJumping = true;
        }

        if (!isGrounded && m_HasDoubleJumpSkill)
        {
            auto tutorial = TutorialManager::GetInstance();
            if (tutorial->IsTutorialDisplayed(TutorialType::DoubleJump))
                tutorial->HideTutorial(TutorialType::DoubleJump);
        }
    }
}

void TPPPlayer::AllowJumping_Gamepad()
{
    m_CanJump_Gamepad = true;
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
        m_IsDashing = true;

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
        m_TeleportDestinationPosition = m_Ghost->GetOwner()->GetTransform()->GetWorldPosition() + glm::vec3(0.0f, 2.0f, 0.0f);

        auto tutorial = TutorialManager::GetInstance();
        if (tutorial->IsTutorialDisplayed(TutorialType::Teleport))
            tutorial->HideTutorial(TutorialType::Teleport);
    }
}

void TPPPlayer::AllowTeleporting()
{
    m_CanTeleport = true;
}

void TPPPlayer::Interact()
{
    if (m_CanInteract && m_Interactable)
    {
        m_Interactable->Interact(this);
        m_CanInteract = false;
    }
}

void TPPPlayer::AllowInteracting()
{
    m_CanInteract = true;
}

void TPPPlayer::LookForInteractable() {
    glm::vec3 origin = m_Owner->GetTransform()->GetWorldPosition();
    glm::vec3 direction = m_Owner->GetTransform()->GetForward();

    RayCastHit hit;
    if (Physics::RayCast(origin, direction, hit, m_InteractDistance, true, m_Owner))
    {
        Actor* actor = hit.Collider->GetOwner();
        if (Ref<Interactable> interactable = actor->GetComponent<Interactable>())
        {
            DisplayInteractionPanel(interactable);
            m_Interactable = interactable;
            return;
        }
    }

    HideInteractionPanel();
    m_Interactable = nullptr;
}

void TPPPlayer::HandleHUD() {
    auto newStaminaBarScale = m_StaminaBar->GetTransform()->GetLocalScale();
    newStaminaBarScale.x = m_StaminaBarDefaultScale.x * m_CharacterController->GetStamina() / 100.0f;
    m_StaminaBar->GetTransform()->SetLocalScale(newStaminaBarScale);
}

void TPPPlayer::UpdateGhostAnimatorParams()
{
    m_Ghost->UpdateAnimatorParams(m_CharacterController->IsGrounded(), m_CharacterController->IsLanding(), m_IsGhostJumping, m_IsGhostDashing, m_IsRunning);
}

void TPPPlayer::HandleAnimator()
{
    float speed = m_CharacterController->GetMovementSpeed() * 7.0f;
    printf("speed %f \n", speed);

    speed = glm::clamp(speed, 0.0f, 0.99f);

    m_CharacterAnimator->SetFloatParameter("Speed", speed);

    bool isGrounded = m_CharacterController->IsGrounded();
    bool isLanding = m_CharacterController->IsLanding();

    m_CharacterAnimator->SetBoolParameter("IsGrounded", isLanding);

    m_CharacterAnimator->SetBoolParameter("IsLanding", isLanding);

    m_CharacterAnimator->SetBoolParameter("IsDashing", m_IsDashing);
    m_CharacterAnimator->SetBoolParameter("IsJumping", m_IsJumping);
    m_CharacterAnimator->SetBoolParameter("IsSprinting", m_IsRunning);
       

    //printf("grounded: %i \n", (int)isGrounded);
    //printf("landing: %i \n", (int)isLanding);
    //printf("dash: %i \n", (int)m_IsDashing);
    
    m_LastFrameIsGrounded = isGrounded;
}
