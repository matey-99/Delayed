#include "Button.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Scene/Scene.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/AudioSourceComponent.h"
#include "Assets/AssetManager.h"

Button::Button(Actor* owner)
	: GameComponent(owner)
{
	m_IsPressed = false;
	m_TriggeringActorsCount = 0;

	m_NormalMaterial = AssetManager::LoadMaterial("Materials/Default.mat");
	m_PressedMaterial = AssetManager::LoadMaterial("Materials/Default.mat");
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
		ENGINE_WARN("Platform is null!");
		return;
	}
	m_Platform = platformActor->GetComponent<Platform>();

	m_AudioSource = GetOwner()->GetComponent<AudioSourceComponent>();

	for (auto& connectedButtonID : m_ConnectedButtonsIDs)
	{
		m_ConnectedButtons.push_back(m_Owner->GetScene()->GetComponent<Button>(connectedButtonID));
	}
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

	bool shouldPlatformBeActive = true;
	for (auto& connectedButton : m_ConnectedButtons)
	{
		if (!connectedButton->IsPressed())
			shouldPlatformBeActive = false;
	}

	m_Platform->SetActive(shouldPlatformBeActive);
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

	bool shouldPlatformBeActive = false;
	for (auto& connectedButton : m_ConnectedButtons)
	{
		if (connectedButton->IsPressed())
			shouldPlatformBeActive = true;
	}

	m_Platform->SetActive(shouldPlatformBeActive);
}
