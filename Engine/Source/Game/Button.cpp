#include "Button.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Scene/Scene.h"
#include "Scene/Component/StaticMeshComponent.h"

Button::Button(Actor* owner)
	: GameComponent(owner)
{
	m_IsPressed = false;
	m_TriggeringActorsCount = 0;
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
}

void Button::Destroy()
{
}

void Button::OnTriggerEnter(Ref<ColliderComponent> other)
{
	if (other->GetOwner()->GetComponent<Player>() || other->GetOwner()->GetComponent<Ghost>())
	{
		m_TriggeringActorsCount++;
		Handle();
	}
}

void Button::OnTriggerExit(Ref<ColliderComponent> other)
{
	if (other->GetOwner()->GetComponent<Player>() || other->GetOwner()->GetComponent<Ghost>())
	{
		m_TriggeringActorsCount--;
		Handle();
	}
}

void Button::Handle()
{
	if (m_TriggeringActorsCount > 0)
		Press();
	else
		Release();
}

void Button::Press()
{
	m_IsPressed = true;

	if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
		mesh->ChangeMaterial(0, m_PressedMaterial);

	m_Platform->SetActive(true);
}

void Button::Release()
{
	m_IsPressed = false;

	if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
		mesh->ChangeMaterial(0, m_NormalMaterial);

	m_Platform->SetActive(false);
}
