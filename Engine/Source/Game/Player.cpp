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

Player::Player(Actor* owner)
	: GameComponent(owner)
{
	m_DashCooldown = 1.0f;
	m_TeleportCooldown = 2.0f;
	m_TeleportTime = 0.05f;

	m_MoveDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);

	m_IsRunning = false;
	m_IsSlowedDown = false;
	m_IsTeleporting = false;
	m_CanJump = true;
	m_CanDash = true;
	m_HasDoubleJumpSkill = false;
	m_HasDashSkill = false;
	m_HasTeleportSkill = false;

	m_DashCooldownTimer = 0.0f;
	m_TeleportCooldownTimer = 0.0f;

	m_LastPosition = glm::vec3(0.0f);
	m_MoveDirectionCopy = glm::vec3(0.0f);
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

	input->BindAction("Run", InputEvent::Press, &Player::RunOn, this);
	input->BindAction("Run", InputEvent::Release, &Player::RunOff, this);

	input->BindAction("Dash", InputEvent::Press, &Player::Dash, this);
	input->BindAction("Dash", InputEvent::Release, &Player::AllowDashing, this);

	input->BindAction("Teleport", InputEvent::Press, &Player::Teleport, this);
	input->BindAction("Teleport", InputEvent::Release, &Player::AllowTeleporting, this);

	input->SetInputMode(InputMode::Player);

	m_CharacterController = m_Owner->AddComponent<CharacterController>();
	m_Camera = m_Owner->GetScene()->GetComponent<CameraComponent>(m_CameraID);
	m_Ghost = m_Owner->GetScene()->FindActor(m_GhostID);
	m_Trail = m_Owner->GetScene()->GetComponent<Trail>(m_TrailID);
	m_StaminaBar = m_Owner->GetScene()->FindActor(m_StaminaBarID);

	m_CharacterController->SetHeadDefaultPosition(m_Camera->GetOwner()->GetTransform()->GetLocalPosition());

	m_LastCheckpointPosition = m_Owner->GetTransform()->GetWorldPosition();
	m_StaminaBarDefaultScale = m_StaminaBar->GetTransform()->GetLocalScale();
}

void Player::Update(float deltaTime)
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

	if (Math::Magnitude(m_MoveDirection) > 0.0f)
		m_MoveDirection = Math::Normalize(m_MoveDirection);

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

	return data;
}

void Player::Load(const SaveData& data)
{
	m_HasDoubleJumpSkill = data.BoolFields.find("HasDoubleJumpSkill")->second;
	m_HasDashSkill = data.BoolFields.find("HasDashSkill")->second;
	m_HasTeleportSkill = data.BoolFields.find("HasTeleportSkill")->second;
	m_LastCheckpointPosition = data.Vector3Fields.find("LastCheckpointPosition")->second;

	BackToLastCheckpoint();
}

void Player::SetLastCheckpoint(Checkpoint* checkpoint)
{
	m_LastCheckpointPosition = checkpoint->GetOwner()->GetTransform()->GetWorldPosition();
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

void Player::MoveForward(float value)
{
	AddMovementInput(m_Owner->GetTransform()->GetForward(), value);
}

void Player::MoveRight(float value)
{
	AddMovementInput(m_Owner->GetTransform()->GetRight(), value);
}

void Player::Turn(float value)
{
	m_Rotation.y += value;
}

void Player::LookUp(float value)
{
	m_Rotation.x += value;
}

void Player::Jump()
{
	if (m_CanJump)
	{
		bool isGrounded = m_CharacterController->IsGrounded();
		if (isGrounded || m_HasDoubleJumpSkill)
		{
			m_CharacterController->Jump();
			m_CanJump = false;
		}
	}
}

void Player::AllowJumping()
{
	m_CanJump = true;
}

void Player::RunOn()
{
	m_IsRunning = true;
}

void Player::RunOff()
{
	m_IsRunning = false;
}

void Player::Dash()
{
	if (m_CanDash && m_HasDashSkill && m_DashCooldownTimer == 0.0f)
	{
		m_CharacterController->Dash();
		m_CanDash = false;
		m_DashCooldownTimer = m_DashCooldown;
	}
}

void Player::AllowDashing()
{
	m_CanDash = true;
}

void Player::Teleport()
{
	if (m_CanTeleport && m_HasTeleportSkill && m_TeleportCooldownTimer == 0.0f)
	{
		m_IsTeleporting = true;
		m_CanTeleport = false;
		m_TeleportCooldownTimer = m_TeleportCooldown;
		m_TeleportDestinationPosition = m_Ghost->GetTransform()->GetWorldPosition();
	}
}

void Player::AllowTeleporting()
{
	m_CanTeleport = true;
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
	auto newStaminaBarScale = m_StaminaBar->GetTransform()->GetLocalScale();
	newStaminaBarScale.x = m_StaminaBarDefaultScale.x * m_CharacterController->GetStamina() / 100.0f;
	m_StaminaBar->GetTransform()->SetLocalScale(newStaminaBarScale);
}

void Player::AddMovementInput(glm::vec3 direction, float value)
{
	m_MoveDirection += (direction * value);
}
