#include "PlayerComponent.h"

#include "Scene/Actor.h"
#include "Input/Input.h"
#include "Math/Math.h"

PlayerComponent::PlayerComponent(Actor* owner)
	: Component(owner)
{
	m_MoveDirection = glm::vec3(0.0f);
	m_MovementSpeed = 15.0f;
}

void PlayerComponent::Start()
{
	auto input = Input::GetInstance();

	input->BindAxis("Player_MoveForward", std::bind(&PlayerComponent::MoveForward, this, std::placeholders::_1));
	input->BindAxis("Player_MoveRight", std::bind(&PlayerComponent::MoveRight, this, std::placeholders::_1));
}

void PlayerComponent::Update(float deltaTime)
{
	if (Math::Magnitude(m_MoveDirection) > 0.0f)
		m_MoveDirection = Math::Normalize(m_MoveDirection);

	glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
	newPosition += m_MoveDirection * m_MovementSpeed * deltaTime;

	m_Owner->GetTransform()->SetLocalPosition(newPosition);

	// Reset move direction
	m_MoveDirection = glm::vec3(0.0f);
}

void PlayerComponent::Destroy()
{
}

void PlayerComponent::MoveForward(float value)
{
	AddMovementInput(Math::ForwardVector, -value);
}

void PlayerComponent::MoveRight(float value)
{
	AddMovementInput(Math::RightVector, value);
}

void PlayerComponent::AddMovementInput(glm::vec3 direction, float value)
{
	m_MoveDirection += (direction * value);
	//m_MoveDirection = glm::normalize(m_MoveDirection);
}
