#include "Player.h"

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
#include "GhostPathElement.h"

Player::Player(Actor* owner)
	: GameComponent(owner)
{
	m_MoveDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);

	m_IsRunning = false;
	m_CanJump = true;

	m_MovementSpeed = 0.0f;
	m_WalkSpeed = 15.0f;
	m_RunSpeed = 25.0f;

	m_RotateSpeed = 8.0f;
	m_LookUpLimit = 80.0f;
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

	auto cameraActor = m_Owner->GetScene()->FindActor(m_CameraID);
	if (!cameraActor)
	{
		DEBUG_LOG("Camera is null!");
		return;
	}
	m_Camera = cameraActor->GetComponent<CameraComponent>();

}

void Player::Update(float deltaTime)
{
	if (Math::Magnitude(m_MoveDirection) > 0.0f)
		m_MoveDirection = Math::Normalize(m_MoveDirection);

	// Player move
	m_MovementSpeed = m_IsRunning ? m_RunSpeed : m_WalkSpeed;
	m_CharacterController->Move(m_MoveDirection * m_MovementSpeed * deltaTime);

	// Player rotation
	glm::vec3 newRotation = m_Owner->GetTransform()->GetLocalRotation();
	newRotation.y += m_Rotation.y * m_RotateSpeed * deltaTime;
	m_Owner->GetTransform()->SetLocalRotation(newRotation);

	// Camera rotation
	auto cameraTransform = m_Camera->GetOwner()->GetTransform();
	glm::vec3 newCameraRotation = cameraTransform->GetLocalRotation();
	newCameraRotation.x -= m_Rotation.x * m_RotateSpeed * deltaTime;
	newCameraRotation.x = glm::clamp(newCameraRotation.x, -m_LookUpLimit, m_LookUpLimit);
	cameraTransform->SetLocalRotation(newCameraRotation);

	// Reset move direction & rotation
	m_MoveDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);

	//m_Owner->GetScene()->SpawnActor<GhostPathElement>(m_Owner->GetTransform()->GetWorldPosition());
}

void Player::MoveForward(float value)
{
	glm::vec3 newDirection = m_Owner->GetTransform()->GetForward();
	newDirection.x *= -1.0f;
	newDirection.y *= -1.0f;

	AddMovementInput(newDirection, value);
}

void Player::MoveRight(float value)
{
	glm::vec3 newDirection = m_Owner->GetTransform()->GetRight();
	newDirection.x *= -1.0f;
	newDirection.y *= -1.0f;

	AddMovementInput(newDirection, value);
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
