#include "PlayerComponent.h"

#include "Scene/Entity.h"
#include "Input/Input.h"

PlayerComponent::PlayerComponent(Entity* owner)
	: InGameComponent(owner)
{
}

void PlayerComponent::Begin()
{
}

void PlayerComponent::Update()
{
}

void PlayerComponent::Destroy()
{
}

void PlayerComponent::BeginPlay()
{
	auto input = Input::GetInstance();

	input->BindInput("Player0_MoveLeft", std::bind(&PlayerComponent::MoveLeft, this));
	input->BindInput("Player0_MoveRight", std::bind(&PlayerComponent::MoveRight, this));
	input->BindInput("Player0_MoveForward", std::bind(&PlayerComponent::MoveForward, this));
	input->BindInput("Player0_MoveBackward", std::bind(&PlayerComponent::MoveBackward, this));
	input->BindInput("Player0_RotateHeadLeft", std::bind(&PlayerComponent::RotateHeadLeft, this));
	input->BindInput("Player0_RotateHeadRight", std::bind(&PlayerComponent::RotateHeadRight, this));


}

void PlayerComponent::Tick(float deltaTime)
{
}

void PlayerComponent::EndPlay()
{
	auto input = Input::GetInstance();

	input->ClearBinding("Player0_MoveLeft");
	input->ClearBinding("Player0_MoveRight");
	input->ClearBinding("Player0_MoveForward");
	input->ClearBinding("Player0_MoveBackward");
}

void PlayerComponent::MoveLeft()
{
	glm::vec3 newPosition = m_Owner->GetTransform().LocalPosition;
	newPosition.x -= 0.3f;
	
	m_Owner->SetLocalPosition(newPosition);
}

void PlayerComponent::MoveRight()
{
	glm::vec3 newPosition = m_Owner->GetTransform().LocalPosition;
	newPosition.x += 0.3f;

	m_Owner->SetLocalPosition(newPosition);
}

void PlayerComponent::MoveForward()
{
	glm::vec3 newPosition = m_Owner->GetTransform().LocalPosition;
	newPosition.z -= 0.3f;

	m_Owner->SetLocalPosition(newPosition);
}

void PlayerComponent::MoveBackward()
{
	glm::vec3 newPosition = m_Owner->GetTransform().LocalPosition;
	newPosition.z += 0.3f;

	m_Owner->SetLocalPosition(newPosition);
}

void PlayerComponent::RotateHeadLeft()
{
	auto children = m_Owner->GetChildren();
	for (auto child : children)
	{
		if (child->GetName() == "Head")
		{
			glm::vec3 newRotation = child->GetTransform().LocalRotation;
			newRotation.y += 1.0f;

			child->SetLocalRotation(newRotation);
			break;
		}
	}
}

void PlayerComponent::RotateHeadRight()
{
	auto children = m_Owner->GetChildren();
	for (auto child : children)
	{
		if (child->GetName() == "Head")
		{
			glm::vec3 newRotation = child->GetTransform().LocalRotation;
			newRotation.y -= 1.0f;

			child->SetLocalRotation(newRotation);
			break;
		}
	}
}
