#include "MainMenu.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/UI/ButtonComponent.h"
#include "Application.h"
#include "Scene/SceneManager.h"

MainMenu::MainMenu(Actor* owner)
	: Component(owner)
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
		DEBUG_LOG("PlayButton is null!");
		return;
	}
	auto optionsButtonActor = m_Owner->GetScene()->FindActor(m_OptionsButtonID);
	if (!optionsButtonActor)
	{
		DEBUG_LOG("OptionsButton is null!");
		return;
	}
	auto exitButtonActor = m_Owner->GetScene()->FindActor(m_ExitButtonID);
	if (!exitButtonActor)
	{
		DEBUG_LOG("ExitButton is null!");
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
	SceneManager::GetInstance()->LoadScene("Scenes/CollisionTesting.scene");
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
