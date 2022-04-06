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

Player::Player(Actor* owner)
	: GameComponent(owner)
{
	m_MoveDirection = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
	m_MovementSpeed = 0.0f;
	m_RotateSpeed = 8.0f;
	m_LookUpLimit = 80.0f;
	smooth = 0.95f;
	t_MovementSpeed = 35.0f;
	m_IsGrounded = false;
}

void Player::Start()
{
	auto input = Input::GetInstance();

	input->BindAxis("Player_MoveForward", std::bind(&Player::MoveForward, this, std::placeholders::_1));
	input->BindAxis("Player_MoveRight", std::bind(&Player::MoveRight, this, std::placeholders::_1));
	input->BindAxis("Player_Turn", std::bind(&Player::Turn, this, std::placeholders::_1));
	input->BindAxis("Player_LookUp", std::bind(&Player::LookUp, this, std::placeholders::_1));

	input->BindAction("Jump", std::bind(&Player::Jump, this));

	//input->SetInputMode(InputMode::Player);

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
	//FIXME player each frame fires Event from transform, this is temporary fix
	if (Math::Magnitude(m_MoveDirection) == 0.0f && Math::Magnitude(m_Rotation) == 0.0f)
		return;

	if (Math::Magnitude(m_MoveDirection) > 0.0f)
		m_MoveDirection = Math::Normalize(m_MoveDirection);

	m_MovementSpeed = t_MovementSpeed * (1.0f - smooth) + m_MovementSpeed * smooth;

	// Player position
	glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
	newPosition += m_MoveDirection * m_MovementSpeed * deltaTime;
	m_Owner->GetTransform()->SetLocalPosition(newPosition);

	

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
	auto rb = GetOwner()->GetComponent<RigidBodyComponent>();
	rb->AddForce(glm::vec3(0.0f, 10.0f, 0.0f));
}

bool Player::IsGrounded()
{
	return false;
}

void Player::AddMovementInput(glm::vec3 direction, float value)
{
	m_MoveDirection += (direction * value);
}
