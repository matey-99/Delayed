#include "Player.h"

#include "Scene/Actor.h"
#include "Input/Input.h"
#include "Math/Math.h"
#include "Time/Time.h"

Player::Player(Actor* owner)
	: Component(owner)
{
	m_MoveDirection = glm::vec3(0.0f);
	m_MovementSpeed = 15.0f;
}

void Player::Start()
{
	auto input = Input::GetInstance();

	input->BindAxis("Player_MoveForward", std::bind(&Player::MoveForward, this, std::placeholders::_1));
	input->BindAxis("Player_MoveRight", std::bind(&Player::MoveRight, this, std::placeholders::_1));
//	input->BindAxis("Player_Turn", std::bind(&PlayerComponent::MoveRight, this, std::placeholders::_1));
//	input->BindAxis("Player_LookUp", std::bind(&PlayerComponent::MoveForward, this, std::placeholders::_1));
}

void Player::Update(float deltaTime)
{
	//FIXME player each frame fires Event from transform, this is temporary fix
	if (Math::Magnitude(m_MoveDirection) == 0.0f)
		return;

	if (Math::Magnitude(m_MoveDirection) > 0.0f)
		m_MoveDirection = Math::Normalize(m_MoveDirection);

	glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
	newPosition += m_MoveDirection * m_MovementSpeed * deltaTime;

	m_Owner->GetTransform()->SetLocalPosition(newPosition);

	// Reset move direction
	m_MoveDirection = glm::vec3(0.0f);
}

void Player::FixedUpdate()
{

}

void Player::Destroy()
{
}

void Player::MoveForward(float value)
{
	AddMovementInput(Math::ForwardVector, value);
}

void Player::MoveRight(float value)
{
	AddMovementInput(Math::RightVector, value);
}

void Player::AddMovementInput(glm::vec3 direction, float value)
{
	m_MoveDirection += (direction * value);
	//m_MoveDirection = glm::normalize(m_MoveDirection);
}
