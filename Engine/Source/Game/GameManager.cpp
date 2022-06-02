#include "GameManager.h"

#include "Input/Input.h"
#include "Time/Time.h"

GameManager* GameManager::s_Instance{};

GameManager::GameManager(Actor* owner)
	: GameComponent(owner)
{
	m_IsGamePaused = false;
	m_CanHandleGameState = true;
}

void GameManager::Start()
{
	if (!s_Instance)
		s_Instance = this;

	m_MainMenu = m_Owner->GetScene()->FindActor(m_MainMenuID);

	auto input = Input::GetInstance();
	input->BindAction("Exit", InputEvent::Press, &GameManager::HandleGameState, this);
	input->BindAction("Exit", InputEvent::Release, &GameManager::EnableHandleGameState, this);
}

void GameManager::PauseGame()
{
	m_IsGamePaused = true;

	m_MainMenu->SetEnabled(true);
	Input::GetInstance()->SetInputMode(InputMode::UI);
}

void GameManager::ResumeGame()
{
	m_IsGamePaused = false;

	m_MainMenu->SetEnabled(false);
	Input::GetInstance()->SetInputMode(InputMode::Player);
}

void GameManager::HandleGameState()
{
	if (!m_CanHandleGameState)
		return;

	if (m_IsGamePaused)
		ResumeGame();
	else
		PauseGame();

	m_CanHandleGameState = false;
}

void GameManager::EnableHandleGameState()
{
	m_CanHandleGameState = true;
}
