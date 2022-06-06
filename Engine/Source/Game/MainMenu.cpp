#include "MainMenu.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/UI/ButtonComponent.h"
#include "Application.h"
#include "Scene/SceneManager.h"
#include "GameManager.h"

MainMenu::MainMenu(Actor* owner)
	: GameComponent(owner)
{
	m_PlayButtonID = 0;
	m_OptionsButtonID = 0;
	m_ExitButtonID = 0;
}

MainMenu::~MainMenu()
{
}

void MainMenu::Start()
{
	auto playButtonActor = m_Owner->GetScene()->FindActor(m_PlayButtonID);
	if (!playButtonActor)
	{
		ENGINE_WARN("PlayButton is null!");
		return;
	}
	auto optionsButtonActor = m_Owner->GetScene()->FindActor(m_OptionsButtonID);
	if (!optionsButtonActor)
	{
		ENGINE_WARN("OptionsButton is null!");
		return;
	}
	auto exitButtonActor = m_Owner->GetScene()->FindActor(m_ExitButtonID);
	if (!exitButtonActor)
	{
		ENGINE_WARN("ExitButton is null!");
		return;
	}

	m_PlayButton = playButtonActor->GetComponent<ButtonComponent>();
	m_PlayButton->OnReleased.Add(&MainMenu::Play, this);

	m_OptionsButton = optionsButtonActor->GetComponent<ButtonComponent>();
	m_OptionsButton->OnReleased.Add(&MainMenu::OpenOptions, this);

	m_ExitButton = exitButtonActor->GetComponent<ButtonComponent>();
	m_ExitButton->OnReleased.Add(&MainMenu::Exit, this);
}

void MainMenu::Update(float deltaTime)
{

}

void MainMenu::Destroy()
{

}

void MainMenu::Play()
{
	if (auto gm = m_Owner->GetScene()->FindActor("Game Manager"))
		gm->GetComponent<GameManager>()->ResumeGame();
	else
		SceneManager::GetInstance()->LoadScene("Scenes/Transition.scene");
}

void MainMenu::OpenOptions()
{
}

void MainMenu::CloseOptions()
{
}

void MainMenu::Exit()
{
	Application::Exit();
}
