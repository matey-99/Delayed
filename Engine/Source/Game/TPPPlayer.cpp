#include "TPPPlayer.h"

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
#include "TPPCharacterController.h"
#include "Checkpoint.h"
#include "CameraController.h"

TPPPlayer::TPPPlayer(Actor* owner)
	: GameComponent(owner)
{
	m_MoveDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);

	m_IsRunning = false;
	m_CanJump = true;
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

	input->SetInputMode(InputMode::Player);

	m_CharacterController = m_Owner->AddComponent<TPPCharacterController>();

	m_LastCheckpointPosition = m_Owner->GetTransform()->GetWorldPosition();

    m_CameraController = m_Owner->GetScene()->GetComponent<CameraController>(m_CameraControllerID);
}

void TPPPlayer::Update(float deltaTime)
{
	if (Math::Magnitude(m_MoveDirection) > 0.0f){
        m_MoveDirection = Math::Normalize(m_MoveDirection);
        m_CharacterController->Rotate(m_CameraController, m_InputDirection, deltaTime);
    }

	CharacterMovementParams params;
	params.IsRunning = m_IsRunning;

	m_CharacterController->Move(m_MoveDirection, params, deltaTime);


	// Reset move direction & rotation
	m_MoveDirection = glm::vec3(0.0f);
    m_InputDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
}

void TPPPlayer::SetLastCheckpoint(Checkpoint* checkpoint)
{
	m_LastCheckpointPosition = checkpoint->GetOwner()->GetTransform()->GetWorldPosition();
}

void TPPPlayer::BackToLastCheckpoint()
{
	auto newPosition = m_LastCheckpointPosition;
	newPosition.y += m_Owner->GetComponent<BoxColliderComponent>()->GetBoundingBox().Extents.y;

	m_Owner->GetTransform()->SetWorldPosition(newPosition);
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

void TPPPlayer::Turn(float value)
{
	m_Rotation.y += value;
}

void TPPPlayer::LookUp(float value)
{
	m_Rotation.x += value;
}

void TPPPlayer::Jump()
{
	if (m_CanJump)
	{
		m_CharacterController->Jump();
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

void TPPPlayer::AddMovementInput(glm::vec3 direction, float value)
{
	m_MoveDirection += (direction * value);
}
