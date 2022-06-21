#include "Button.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Scene/Scene.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/AudioSourceComponent.h"
#include "Assets/AssetManager.h"
#include "Material/MaterialInstance.h"

Button::Button(Actor* owner)
	: GameComponent(owner)
{
	m_IsPressed = false;
	m_TriggeringActorsCount = 0;

	m_EmissionStrength = 5.0f;
	m_EmissionTime = 0.2f;
	m_EmissionTimer = 0.0f;

	m_PlatformDelayTime = 0.0f;
	m_PlatformDelayTimerHandle = 0;

	m_Material = nullptr;
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

	m_Platform = m_Owner->GetScene()->GetComponent<Platform>(m_PlatformID);

	if (m_Platform)
		m_Platform->AddButton(this);

	m_AudioSource = GetOwner()->GetComponent<AudioSourceComponent>();

	if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
	{
		Ref<Material> material = mesh->GetMaterials()[0];
		m_Material = MaterialInstance::Create(material);

		mesh->SetMaterial(0, m_Material);
	}

	for (auto& connectedButtonID : m_ConnectedButtonsIDs)
	{
		m_ConnectedButtons.push_back(m_Owner->GetScene()->GetComponent<Button>(connectedButtonID));
	}
}

void Button::Update(float deltaTime)
{
	if (m_IsPressed)
	{
		if (m_EmissionTimer < m_EmissionTime)
		{
			m_EmissionTimer += deltaTime;
			float step = m_EmissionStrength * deltaTime / m_EmissionTime;

			float currentEmissive = m_Material->GetFloatParameter("u_Material.emissiveStrength");
			m_Material->SetFloatParameter("u_Material.emissiveStrength", currentEmissive + step);
		}
	}
	else
	{
		if (m_EmissionTimer > 0.0f)
		{
			m_EmissionTimer -= deltaTime;
			float step = m_EmissionStrength * deltaTime / m_EmissionTime;

			float currentEmissive = m_Material->GetFloatParameter("u_Material.emissiveStrength");
			m_Material->SetFloatParameter("u_Material.emissiveStrength", currentEmissive - step);
		}
	}
}

void Button::OnTriggerEnter(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Player>() || other->GetOwner()->GetComponent<Ghost>())
	{
		if (m_TriggeringActorsCount == 0)
			Press();

		m_TriggeringActorsCount++;
	}
}

void Button::OnTriggerExit(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Player>() || other->GetOwner()->GetComponent<Ghost>())
	{
		m_TriggeringActorsCount--;
		if (m_TriggeringActorsCount == 0)
		{
			if (m_PlatformDelayTime > 0.0f)
			{
				Event e;
				e.Add(&Button::Release, this);
				m_PlatformDelayTimerHandle = TimerManager::GetInstance()->SetTimer(e, m_PlatformDelayTime, false);
			}
			else
				Release();
		}
	}
}

void Button::Press()
{
	m_IsPressed = true;

	if (m_AudioSource)
	{
		m_AudioSource->ChangeSound("../../../Content/Audio/Placeholders/platform_up_placeholder.wav");
		m_AudioSource->PlaySound();
	}

	if (m_PlatformDelayTimerHandle)
		TimerManager::GetInstance()->ClearTimer(m_PlatformDelayTimerHandle);
}

void Button::Release()
{
	m_IsPressed = false;

	if (m_AudioSource)
	{
		m_AudioSource->ChangeSound("../../../Content/Audio/Placeholders/platform_down_placeholder.wav");
		m_AudioSource->PlaySound();
	}

	if (m_PlatformDelayTimerHandle)
		TimerManager::GetInstance()->ClearTimer(m_PlatformDelayTimerHandle);
}
