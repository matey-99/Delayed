#include "Button.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Scene/Scene.h"

Button::Button(Actor* owner)
	: GameComponent(owner)
{
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

	auto platformActor = m_Owner->GetScene()->FindActor(m_PlatformID);
	if (!platformActor)
	{
		DEBUG_LOG("Platform is null!");
		return;
	}
	m_Platform = platformActor->GetComponent<Platform>();
}

void Button::Update(float deltaTime)
{
	if (m_ConnectedButtons.empty())
	{
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

		m_Platform->SetActive(true);
	}
}

void Button::OnTriggerExit(Ref<ColliderComponent> other)
{
	if (other->GetOwner()->GetComponent<Player>())
	{
		m_IsPressed = false;

		m_Platform->SetActive(false);
	}
}
