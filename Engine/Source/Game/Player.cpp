#include "Player.h"

#include <glm/gtx/rotate_vector.hpp>

#include "Scene/Actor.h"
#include "Input/Input.h"
#include "Math/Math.h"
#include "Math/Ray.h"
#include "Time/Time.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Scene/Component/Collider/BoxColliderComponent.h"
#include "Scene/Component/Collider/SphereColliderComponent.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/RigidBodyComponent.h"
#include "Physics/Physics.h"
#include "CharacterController.h"
#include "Trail.h"
#include "Checkpoint.h"
#include "PickableSkill.h"
#include "GameManager.h"
#include "TutorialManager.h"
#include "Interactable.h"
#include "InteractionPanel.h"
#include "Inventory.h"
#include "Ghost.h"

Player::Player(Actor* owner)
	: GameComponent(owner)
{
	m_DashCooldown = 1.0f;
	m_TeleportCooldown = 2.0f;
	m_TeleportTime = 0.05f;
	m_InteractDistance = 10.0f;
	m_GamepadRotationSensitivity = 20.0f;

	m_MoveDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);

	m_IsRunning = false;
	m_IsSlowedDown = false;
	m_IsTeleporting = false;
	m_CanJump = true;
	m_CanJump_Gamepad = true;
	m_CanDash = true;
	m_CanInteract = true;
	m_HasDoubleJumpSkill = false;
	m_HasDashSkill = false;
	m_HasTeleportSkill = false;

	m_DashCooldownTimer = 0.0f;
	m_TeleportCooldownTimer = 0.0f;

	m_LastPosition = glm::vec3(0.0f);
	m_MoveDirectionCopy = glm::vec3(0.0f);

	m_IsDashing = false;
	m_IsJumping = false;

	m_IsGhostJumping = false;
	m_IsGhostDashing = false;
}

void Player::Start()
{
	auto input = Input::GetInstance();

	input->BindAxis("Player_MoveForward", &Player::MoveForward, this);
	input->BindAxis("Player_MoveRight", &Player::MoveRight, this);
	input->BindAxis("Player_Turn", &Player::Turn, this);
	input->BindAxis("Player_LookUp", &Player::LookUp, this);

	input->BindAction("Jump", InputEvent::Press, &Player::Jump, this);
	input->BindAction("Jump", InputEvent::Release, &Player::AllowJumping, this);

	input->BindAction("Jump_Gamepad", InputEvent::Press, &Player::Jump_Gamepad, this);
	input->BindAction("Jump_Gamepad", InputEvent::Release, &Player::AllowJumping_Gamepad, this);

	input->BindAction("Run", InputEvent::Press, &Player::RunOn, this);
	input->BindAction("Run", InputEvent::Release, &Player::RunOff, this);

	input->BindAction("Dash", InputEvent::Press, &Player::Dash, this);
	input->BindAction("Dash", InputEvent::Release, &Player::AllowDashing, this);

	input->BindAction("Teleport", InputEvent::Press, &Player::Teleport, this);
	input->BindAction("Teleport", InputEvent::Release, &Player::AllowTeleporting, this);

	input->BindAction("Interact", InputEvent::Press, &Player::Interact, this);
	input->BindAction("Interact", InputEvent::Release, &Player::AllowInteracting, this);

	input->SetInputMode(InputMode::Player);

	m_CharacterController = m_Owner->AddComponent<CharacterController>();
	m_Inventory = m_Owner->AddComponent<Inventory>();
	m_Camera = m_Owner->GetScene()->GetComponent<CameraComponent>(m_CameraID);
	m_Ghost = m_Owner->GetScene()->GetComponent<Ghost>(m_GhostID);
	m_Trail = m_Owner->GetScene()->GetComponent<Trail>(m_TrailID);
	m_InteractionPanel = m_Owner->GetScene()->GetComponent<InteractionPanel>(m_InteractionPanelID);

	m_CharacterController->SetHeadDefaultPosition(m_Camera->GetOwner()->GetTransform()->GetLocalPosition());

	m_LastCheckpointPosition = m_Owner->GetTransform()->GetWorldPosition();
}

void Player::Update(float deltaTime)
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
			auto newPosition = Math::Smoothstep(currentPosition, m_TeleportDestinationPosition, 1 / m_TeleportTime * deltaTime);
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
	m_CharacterController->Rotate(m_Camera, m_Rotation, deltaTime);
	m_CharacterController->MoveHead(params, m_Camera, deltaTime);

	HandleSkillsCooldowns(deltaTime);
	HandleHUD();

	m_LastPosition = currentPosition;

	m_IsGhostJumping = m_IsJumping;
	m_IsGhostDashing = m_IsDashing;

	if (GetMovementSpeed() <= 0.8f)
		m_IsDashing = false;
	m_IsJumping = false;

	// Reset move direction & rotation
	m_MoveDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
}

const SaveData Player::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "HasDoubleJumpSkill", m_HasDoubleJumpSkill });
	data.BoolFields.insert({ "HasDashSkill", m_HasDashSkill });
	data.BoolFields.insert({ "HasTeleportSkill", m_HasTeleportSkill });
	data.Vector3Fields.insert({ "LastCheckpointPosition", m_LastCheckpointPosition });

	for (int i = 0; i < SPACESHIP_PARTS_COUNT; i++)
		data.BoolFields.insert({ std::to_string(i), m_Inventory->IsItemCollected((SpaceshipPartType)i) });

	return data;
}

void Player::Load(const SaveData& data)
{
	m_HasDoubleJumpSkill = data.BoolFields.find("HasDoubleJumpSkill")->second;
	m_HasDashSkill = data.BoolFields.find("HasDashSkill")->second;
	m_HasTeleportSkill = data.BoolFields.find("HasTeleportSkill")->second;
	m_LastCheckpointPosition = data.Vector3Fields.find("LastCheckpointPosition")->second;

	for (int i = 0; i < SPACESHIP_PARTS_COUNT; i++)
	{
		bool collected = data.BoolFields.find(std::to_string(i))->second;
		if (collected)
			m_Inventory->AddItem((SpaceshipPartType)i);
		else if (!collected && m_Inventory->IsItemCollected((SpaceshipPartType)i))
			m_Inventory->RemoveItem((SpaceshipPartType)i);
	}

	BackToLastCheckpoint();
}

void Player::SetLastCheckpoint(glm::vec3 position)
{
	m_LastCheckpointPosition = position;
}

void Player::BackToLastCheckpoint()
{
	auto newPosition = m_LastCheckpointPosition;
	newPosition.y += m_Owner->GetComponent<BoxColliderComponent>()->GetBoundingBox().Extents.y;

	m_Owner->GetTransform()->SetWorldPosition(newPosition);
}

void Player::AddSkill(SkillType skill)
{
	switch (skill)
	{
	case SkillType::DoubleJump:
		m_HasDoubleJumpSkill = true;
		break;
	case SkillType::Dash:
		m_HasDashSkill = true;
		break;
	case SkillType::Teleport:
		m_HasTeleportSkill = true;
		break;
	}
}

void Player::SlowDown()
{
	m_IsSlowedDown = true;

	m_CanJump = false;
	m_CanDash = false;
	m_CanTeleport = false;
}

void Player::BackToNormal()
{
	m_IsSlowedDown = false;

	m_CanJump = true;
	m_CanDash = m_HasDashSkill;
	m_CanTeleport = m_HasTeleportSkill;
}

float Player::GetMovementSpeed()
{
	return m_CharacterController->GetMovementSpeed();
}

void Player::MoveForward(float value)
{
	AddMovementInput(m_Owner->GetTransform()->GetForward(), value);

	auto tutorial = TutorialManager::GetInstance();
	if (tutorial->IsTutorialDisplayed(TutorialType::Movement))
		tutorial->HideTutorial(TutorialType::Movement);
}

void Player::MoveRight(float value)
{
	AddMovementInput(m_Owner->GetTransform()->GetRight(), value);

	auto tutorial = TutorialManager::GetInstance();
	if (tutorial->IsTutorialDisplayed(TutorialType::Movement))
		tutorial->HideTutorial(TutorialType::Movement);
}

void Player::Turn(float value)
{
	auto input = Input::GetInstance()->GetCurrentInputType();
	if (input == PlayerInputType::Gamepad)
		value *= m_GamepadRotationSensitivity;

	m_Rotation.y += value;
}

void Player::LookUp(float value)
{
	auto input = Input::GetInstance()->GetCurrentInputType();
	if (input == PlayerInputType::Gamepad)
		value *= m_GamepadRotationSensitivity;

	m_Rotation.x += value;
}

void Player::Jump()
{
	if (!m_Owner->IsEnabled())
		return;

	if (m_CanJump)
	{
		bool isGrounded = m_CharacterController->IsGrounded();
		if (isGrounded || m_HasDoubleJumpSkill)
		{
			m_CharacterController->Jump();
			m_CanJump = false;
			m_IsJumping = true;

			auto tutorial = TutorialManager::GetInstance();
			if (tutorial->IsTutorialDisplayed(TutorialType::Jump))
				tutorial->HideTutorial(TutorialType::Jump);
		}

		if (!isGrounded && m_HasDoubleJumpSkill)
		{
			auto tutorial = TutorialManager::GetInstance();
			if (tutorial->IsTutorialDisplayed(TutorialType::DoubleJump))
				tutorial->HideTutorial(TutorialType::DoubleJump);
		}
	}
}

void Player::AllowJumping()
{
	m_CanJump = true;
}

void Player::Jump_Gamepad()
{
	if (!m_Owner->IsEnabled())
		return;

	if (m_CanJump_Gamepad)
	{
		bool isGrounded = m_CharacterController->IsGrounded();
		if (isGrounded || m_HasDoubleJumpSkill)
		{
			m_CharacterController->Jump();
			m_CanJump_Gamepad = false;
			m_IsJumping = true;

			auto tutorial = TutorialManager::GetInstance();
			if (tutorial->IsTutorialDisplayed(TutorialType::Jump))
				tutorial->HideTutorial(TutorialType::Jump);
		}

		if (!isGrounded && m_HasDoubleJumpSkill)
		{
			auto tutorial = TutorialManager::GetInstance();
			if (tutorial->IsTutorialDisplayed(TutorialType::DoubleJump))
				tutorial->HideTutorial(TutorialType::DoubleJump);
		}
	}
}

void Player::AllowJumping_Gamepad()
{
	m_CanJump_Gamepad = true;
}

void Player::RunOn()
{
	m_IsRunning = true;

	auto tutorial = TutorialManager::GetInstance();
	if (tutorial->IsTutorialDisplayed(TutorialType::Sprint))
		tutorial->HideTutorial(TutorialType::Sprint);
}

void Player::RunOff()
{
	m_IsRunning = false;
}

void Player::Dash()
{
	if (!m_Owner->IsEnabled())
		return;

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

void Player::AllowDashing()
{
	m_CanDash = true;
}

void Player::Teleport()
{
	if (!m_Owner->IsEnabled())
		return;

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

void Player::AllowTeleporting()
{
	m_CanTeleport = true;
}

void Player::Interact()
{
	if (!m_Owner->IsEnabled())
		return;

	if (m_CanInteract && m_Interactable)
	{
		m_Interactable->Interact(this);
		m_CanInteract = false;
	}
}

void Player::AllowInteracting()
{
	m_CanInteract = true;
}

void Player::HandleSkillsCooldowns(float deltaTime)
{
	m_DashCooldownTimer -= deltaTime;
	m_DashCooldownTimer = glm::clamp(m_DashCooldownTimer, 0.0f, m_DashCooldown);

	m_TeleportCooldownTimer -= deltaTime;
	m_TeleportCooldownTimer = glm::clamp(m_TeleportCooldownTimer, 0.0f, m_TeleportCooldown);
}

void Player::HandleHUD()
{

}

void Player::UpdateGhostAnimatorParams()
{
	m_Ghost->UpdateAnimatorParams(m_CharacterController->IsGrounded(), m_CharacterController->IsLanding(), m_IsGhostJumping, m_IsGhostDashing, m_IsRunning);
}

void Player::EnableGhost()
{
	m_Trail->EnableTrailParticlesEmission(true);
	m_Ghost->GetOwner()->SetEnabled(true);
}

void Player::AddMovementInput(glm::vec3 direction, float value)
{
	m_MoveDirection += (direction * value);
}

void Player::LookForInteractable()
{
	glm::vec3 origin = m_Camera->GetWorldPosition();
	glm::vec3 direction = m_Camera->GetFront();

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

void Player::DisplayInteractionPanel(Ref<Interactable> interactable)
{
	m_InteractionPanel->GetOwner()->SetEnabled(true);
	m_InteractionPanel->UpdatePanel(interactable);
}

void Player::HideInteractionPanel()
{
	m_InteractionPanel->GetOwner()->SetEnabled(false);
}