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

Player::Player(Actor* owner)
	: GameComponent(owner)
{
	m_MoveDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);

	m_IsRunning = false;
	m_CanJump = true;
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

	input->SetInputMode(InputMode::Player);

	m_CharacterController = m_Owner->AddComponent<CharacterController>();
	m_Camera = m_Owner->GetScene()->GetComponent<CameraComponent>(m_CameraID);
	m_Trail = m_Owner->GetScene()->GetComponent<Trail>(m_TrailID);

	m_LastCheckpointPosition = m_Owner->GetTransform()->GetWorldPosition();
}

void Player::Update(float deltaTime)
{
	if (Math::Magnitude(m_MoveDirection) > 0.0f)
	{
		m_MoveDirection = Math::Normalize(m_MoveDirection);

		m_Trail->EnableTrailParticlesEmission(true);
	}
	else
		m_Trail->EnableTrailParticlesEmission(false);

	CharacterMovementParams params;
	params.IsRunning = m_IsRunning;

	m_CharacterController->Move(m_MoveDirection, params, deltaTime);
	m_CharacterController->Rotate(m_Camera, m_Rotation, deltaTime);

	// Reset move direction & rotation
	m_MoveDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
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
		m_CharacterController->Jump();
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

void Player::AddMovementInput(glm::vec3 direction, float value)
{
	m_MoveDirection += (direction * value);
}
