#include "TutorialManager.h"

#include "Input/Input.h"
#include "Time/Time.h"

TutorialManager* TutorialManager::s_Instance{};

TutorialManager::TutorialManager(Actor* owner)
	: GameComponent(owner)
{
	m_TutorialEnabled = true;
}

void TutorialManager::Start()
{
	if (!s_Instance)
		s_Instance = this;

	m_DoubleJumpTutorial = m_Owner->GetScene()->FindActor(m_DoubleJumpTutorialID);
	m_DashTutorial = m_Owner->GetScene()->FindActor(m_DashTutorialID);
	m_TeleportTutorial = m_Owner->GetScene()->FindActor(m_TeleportTutorialID);

	if (!m_DoubleJumpTutorial || !m_DashTutorial || !m_TeleportTutorial)
		m_TutorialEnabled = false;
}

void TutorialManager::DisplayTutorial(TutorialType type)
{
	switch (type)
	{
	case TutorialType::DoubleJump:
		m_DoubleJumpTutorial->SetEnabled(true);
		break;
	case TutorialType::Dash:
		m_DashTutorial->SetEnabled(true);
		break;
	case TutorialType::Teleport:
		m_TeleportTutorial->SetEnabled(true);
		break;
	}
}

void TutorialManager::HideTutorial(TutorialType type)
{
	switch (type)
	{
	case TutorialType::DoubleJump:
		m_DoubleJumpTutorial->SetEnabled(false);
		break;
	case TutorialType::Dash:
		m_DashTutorial->SetEnabled(false);
		break;
	case TutorialType::Teleport:
		m_TeleportTutorial->SetEnabled(false);
		break;
	}
}

bool TutorialManager::IsTutorialDisplayed(TutorialType type) const
{
	if (!m_TutorialEnabled)
		return false;

	switch (type)
	{
	case TutorialType::DoubleJump:
		return m_DoubleJumpTutorial->IsEnabled();
	case TutorialType::Dash:
		return m_DashTutorial->IsEnabled();
	case TutorialType::Teleport:
		return m_TeleportTutorial->IsEnabled();
	}

	return false;
}
