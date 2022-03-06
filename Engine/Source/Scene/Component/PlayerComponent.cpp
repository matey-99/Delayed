#include "PlayerComponent.h"

#include "Scene/Actor.h"
#include "Input/Input.h"

PlayerComponent::PlayerComponent(Actor* owner)
	: Component(owner)
{
}

void PlayerComponent::Start()
{
	auto input = Input::GetInstance();

	input->BindInput("Player0_MoveLeft", std::bind(&PlayerComponent::MoveLeft, this));
	input->BindInput("Player0_MoveRight", std::bind(&PlayerComponent::MoveRight, this));
	input->BindInput("Player0_MoveForward", std::bind(&PlayerComponent::MoveForward, this));
	input->BindInput("Player0_MoveBackward", std::bind(&PlayerComponent::MoveBackward, this));
}

void PlayerComponent::Update(float deltaTime)
{
}

void PlayerComponent::Destroy()
{
}

void PlayerComponent::MoveLeft()
{
	glm::vec3 newPosition = m_Owner->GetTransform().LocalPosition;
	newPosition.x -= 0.001f;
	
	m_Owner->SetLocalPosition(newPosition);
}

void PlayerComponent::MoveRight()
{
	glm::vec3 newPosition = m_Owner->GetTransform().LocalPosition;
	newPosition.x += 0.001f;

	m_Owner->SetLocalPosition(newPosition);
}

void PlayerComponent::MoveForward()
{
	glm::vec3 newPosition = m_Owner->GetTransform().LocalPosition;
	newPosition.z -= 0.001f;

	m_Owner->SetLocalPosition(newPosition);
}

void PlayerComponent::MoveBackward()
{
	glm::vec3 newPosition = m_Owner->GetTransform().LocalPosition;
	newPosition.z += 0.001f;

	m_Owner->SetLocalPosition(newPosition);
}
