#include "NotificationManager.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/UI/TextComponent.h"
#include "Time/TimerManager.h"

NotificationManager* NotificationManager::s_Instance{};

NotificationManager::NotificationManager(Actor* owner)
	: GameComponent(owner)
{
}

void NotificationManager::Start()
{
	s_Instance = this;

	m_NotificationText = m_Owner->GetScene()->GetComponent<TextComponent>(m_NotificationTextID);
	m_NotificationText->GetOwner()->SetEnabled(false);
}

void NotificationManager::Notify(std::string message, glm::vec4 color, float timeToHide)
{
	m_NotificationText->SetText(message);
	m_NotificationText->SetNormalColor(color);
	m_NotificationText->GetOwner()->SetEnabled(true);

	Event e;
	e.Add(&NotificationManager::HideNotification, this);
	m_NotificationTimerHandle = TimerManager::GetInstance()->SetTimer(e, timeToHide, false);
}

void NotificationManager::HideNotification()
{
	m_NotificationText->GetOwner()->SetEnabled(false);

	TimerManager::GetInstance()->ClearTimer(m_NotificationTimerHandle);
}
