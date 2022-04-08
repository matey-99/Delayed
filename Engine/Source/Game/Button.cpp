#include "Button.h"

#include "Scene/Actor.h"
#include "Game/Player.h"

Button::Button(Actor* owner)
	: GameComponent(owner)
{
	m_Speed = 255.0f;

	m_IsPressed = false;
}

Button::~Button()
{
}

void Button::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
	{
		collider->OnTriggerEnterDelegate.Add(&Button::OnTriggerEnter, this);
		collider->OnTriggerExitDelegate.Add(&Button::OnTriggerExit, this);
	}
}

void Button::Update(float deltaTime)
{
	if (m_ConnectedButtons.empty())
	{
		float step = m_Speed * deltaTime;

		if (m_IsPressed)
		{
			// TO DO: Make platform moves to m_PlatformRelativeEndPosition
		}
		else
		{
			// TO DO: Make platform moves to m_PlatformRelativeStartPosition
		}
	}
}

void Button::Destroy()
{
}

void Button::OnTriggerEnter(Ref<ColliderComponent> other)
{
	if (other->GetOwner()->GetComponent<Player>())
	{
		m_IsPressed = true;

	}
}

void Button::OnTriggerExit(Ref<ColliderComponent> other)
{
	if (other->GetOwner()->GetComponent<Player>())
	{
		m_IsPressed = false;

	}
}
