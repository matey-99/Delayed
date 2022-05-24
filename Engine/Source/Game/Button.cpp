#include "Button.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Scene/Scene.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/AudioSourceComponent.h"

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
		WARN("Platform is null!");
		return;
	}
	m_Platform = platformActor->GetComponent<Platform>();

	m_AudioSource = GetOwner()->GetComponent<AudioSourceComponent>();
}

void Button::Update(float deltaTime)
{
}

void Button::Destroy()
{
}

void Button::OnTriggerEnter(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Player>() || other->GetOwner()->GetComponent<Ghost>())
	{
		m_TriggeringActorsCount++;
		Handle();
	}
}

void Button::OnTriggerExit(ColliderComponent* other)
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

	if (m_AudioSource)
	{
		m_AudioSource->ChangeSound("../../../Content/Audio/Placeholders/platform_up_placeholder.wav");
		m_AudioSource->PlaySound();
	}

	m_Platform->SetActive(true);
}

void Button::Release()
{
	m_IsPressed = false;

	if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
		mesh->ChangeMaterial(0, m_NormalMaterial);

	if (m_AudioSource)
	{
		m_AudioSource->ChangeSound("../../../Content/Audio/Placeholders/platform_down_placeholder.wav");
		m_AudioSource->PlaySound();
	}

	m_Platform->SetActive(false);
}
