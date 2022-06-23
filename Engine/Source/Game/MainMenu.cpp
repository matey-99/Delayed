#include "MainMenu.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/UI/ButtonComponent.h"
#include "Scene/Component/UI/TextComponent.h"
#include "Application.h"
#include "Scene/SceneManager.h"
#include "GameManager.h"
#include "Audio/AudioSystem.h"
#include "Input/Input.h"
#include "SaveManager.h"

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
	/* Default Panel */
	m_DefaultPanel = m_Owner->GetScene()->FindActor(m_DefaultPanelID);

	m_PlayButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_PlayButtonID);
	m_PlayButton->OnReleased.Add(&MainMenu::Play, this);

	m_ResetButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_ResetButtonID);
	m_ResetButton->OnReleased.Add(&MainMenu::Reset, this);

	m_OptionsButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_OptionsButtonID);
	m_OptionsButton->OnReleased.Add(&MainMenu::OpenOptions, this);

	m_CreditsButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_CreditsButtonID);
	m_CreditsButton->OnReleased.Add(&MainMenu::OpenCredits, this);

	m_ExitButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_ExitButtonID);
	m_ExitButton->OnReleased.Add(&MainMenu::Exit, this);

	/* Options Panel */
	m_OptionsPanel = m_Owner->GetScene()->FindActor(m_OptionsPanelID);

	m_MasterVolumeText = m_Owner->GetScene()->GetComponent<TextComponent>(m_MasterVolumeTextID);
	m_MasterVolumeSlider = m_Owner->GetScene()->FindActor(m_MasterVolumeSliderID);

	m_IncreaseMasterVolumeButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_IncreaseMasterVolumeButtonID);
	m_IncreaseMasterVolumeButton->OnReleased.Add(&MainMenu::IncreaseMasterVolume, this);

	m_DecreaseMasterVolumeButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_DecreaseMasterVolumeButtonID);
	m_DecreaseMasterVolumeButton->OnReleased.Add(&MainMenu::DecreaseMasterVolume, this);

	m_MusicVolumeText = m_Owner->GetScene()->GetComponent<TextComponent>(m_MusicVolumeTextID);
	m_MusicVolumeSlider = m_Owner->GetScene()->FindActor(m_MusicVolumeSliderID);

	m_IncreaseMusicVolumeButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_IncreaseMusicVolumeButtonID);
	m_IncreaseMusicVolumeButton->OnReleased.Add(&MainMenu::IncreaseMusicVolume, this);

	m_DecreaseMusicVolumeButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_DecreaseMusicVolumeButtonID);
	m_DecreaseMusicVolumeButton->OnReleased.Add(&MainMenu::DecreaseMusicVolume, this);

	m_SoundsVolumeText = m_Owner->GetScene()->GetComponent<TextComponent>(m_SoundsVolumeTextID);
	m_SoundsVolumeSlider = m_Owner->GetScene()->FindActor(m_SoundsVolumeSliderID);

	m_IncreaseSoundsVolumeButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_IncreaseSoundsVolumeButtonID);
	m_IncreaseSoundsVolumeButton->OnReleased.Add(&MainMenu::IncreaseSoundsVolume, this);

	m_DecreaseSoundsVolumeButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_DecreaseSoundsVolumeButtonID);
	m_DecreaseSoundsVolumeButton->OnReleased.Add(&MainMenu::DecreaseSoundsVolume, this);

	m_BackFromOptionsButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_BackFromOptionsButtonID);
	m_BackFromOptionsButton->OnReleased.Add(&MainMenu::CloseOptions, this);

	/* Credits Panel */
	m_CreditsPanel = m_Owner->GetScene()->FindActor(m_CreditsPanelID);

	m_BackFromCreditsButton = m_Owner->GetScene()->GetComponent<ButtonComponent>(m_BackFromCreditsButtonID);
	m_BackFromCreditsButton->OnReleased.Add(&MainMenu::CloseCredits, this);

	UpdateOptions();

	std::string sceneName = m_Owner->GetScene()->GetName();
	if (sceneName == "MainMenu" || sceneName == "MainMenu2")
		Input::GetInstance()->SetInputMode(InputMode::UI);
}

void MainMenu::Play()
{
	if (auto gm = m_Owner->GetScene()->FindActor("Game Manager"))
		gm->GetComponent<GameManager>()->ResumeGame();
	else 
	{
		auto saveManager = SaveManager::GetInstance();
		if (m_Owner->GetScene()->GetName() == "MainMenu")
		{
			saveManager->SetLoadGameOnStart(false);
			SceneManager::GetInstance()->LoadScene("Scenes/Intro.scene");
		}
		else if (m_Owner->GetScene()->GetName() == "MainMenu2")
		{
			saveManager->SetLoadGameOnStart(true);
			SceneManager::GetInstance()->LoadScene("Scenes/Transition.scene");
		}
    }
}

void MainMenu::Reset()
{
	SceneManager::GetInstance()->LoadScene("Scenes/MainMenu.scene");
}

void MainMenu::OpenOptions()
{
	m_OptionsPanel->SetEnabled(true);
	m_DefaultPanel->SetEnabled(false);
}

void MainMenu::OpenCredits()
{
	m_CreditsPanel->SetEnabled(true);
	m_DefaultPanel->SetEnabled(false);
}

void MainMenu::CloseOptions()
{
	m_OptionsPanel->SetEnabled(false);
	m_DefaultPanel->SetEnabled(true);
}

void MainMenu::CloseCredits()
{
	m_CreditsPanel->SetEnabled(false);
	m_DefaultPanel->SetEnabled(true);
}

void MainMenu::Exit()
{
	if (auto gm = m_Owner->GetScene()->FindActor("Game Manager"))
	{
		SceneManager::GetInstance()->LoadScene("Scenes/MainMenu2.scene");
	}
	else
		Application::Exit();
}

void MainMenu::IncreaseMasterVolume()
{
	auto audioSystem = AudioSystem::GetInstance();

	float masterVolume = audioSystem->GetChannelGroupVolume(CHANNEL_GROUP::MASTER);
    if (masterVolume < 0.99f)
	    masterVolume += 0.1f;

	audioSystem->SetChannelGroupVolume(CHANNEL_GROUP::MASTER, masterVolume);
	UpdateOptions();
}

void MainMenu::DecreaseMasterVolume()
{
	auto audioSystem = AudioSystem::GetInstance();

	float masterVolume = audioSystem->GetChannelGroupVolume(CHANNEL_GROUP::MASTER);
    if (masterVolume > 0.01f)
	    masterVolume -= 0.1f;

	audioSystem->SetChannelGroupVolume(CHANNEL_GROUP::MASTER, masterVolume);
	UpdateOptions();
}

void MainMenu::IncreaseMusicVolume()
{
	auto audioSystem = AudioSystem::GetInstance();

	float musicVolume = audioSystem->GetChannelGroupVolume(CHANNEL_GROUP::MUSIC);
    if (musicVolume < 0.99f)
	    musicVolume += 0.1f;

	audioSystem->SetChannelGroupVolume(CHANNEL_GROUP::MUSIC, musicVolume);
	UpdateOptions();
}

void MainMenu::DecreaseMusicVolume()
{
	auto audioSystem = AudioSystem::GetInstance();

	float musicVolume = audioSystem->GetChannelGroupVolume(CHANNEL_GROUP::MUSIC);
    if (musicVolume > 0.01f)
	    musicVolume -= 0.1f;

	audioSystem->SetChannelGroupVolume(CHANNEL_GROUP::MUSIC, musicVolume);
	UpdateOptions();
}

void MainMenu::IncreaseSoundsVolume()
{
	auto audioSystem = AudioSystem::GetInstance();

	float soundsVolume = audioSystem->GetChannelGroupVolume(CHANNEL_GROUP::SFX);
    if (soundsVolume < 0.99f)
	    soundsVolume += 0.1f;

	audioSystem->SetChannelGroupVolume(CHANNEL_GROUP::SFX, soundsVolume);
	UpdateOptions();
}

void MainMenu::DecreaseSoundsVolume()
{
	auto audioSystem = AudioSystem::GetInstance();

	float soundsVolume = audioSystem->GetChannelGroupVolume(CHANNEL_GROUP::SFX);
    if (soundsVolume > 0.01f)
	    soundsVolume -= 0.1f;

	audioSystem->SetChannelGroupVolume(CHANNEL_GROUP::SFX, soundsVolume);
	UpdateOptions();
}

void MainMenu::UpdateOptions()
{
	auto audioSystem = AudioSystem::GetInstance();

	float masterVolume = audioSystem->GetChannelGroupVolume(CHANNEL_GROUP::MASTER);
	m_MasterVolumeText->SetText(std::to_string((int)(masterVolume * 100)));
	m_MasterVolumeSlider->GetTransform()->SetLocalScale(glm::vec3(masterVolume, 1.0f, 1.0f));

	float musicVolume = audioSystem->GetChannelGroupVolume(CHANNEL_GROUP::MUSIC);
	m_MusicVolumeText->SetText(std::to_string((int)(musicVolume * 100)));
	m_MusicVolumeSlider->GetTransform()->SetLocalScale(glm::vec3(musicVolume, 1.0f, 1.0f));

	float soundsVolume = audioSystem->GetChannelGroupVolume(CHANNEL_GROUP::SFX);
	m_SoundsVolumeText->SetText(std::to_string((int)(soundsVolume * 100)));
	m_SoundsVolumeSlider->GetTransform()->SetLocalScale(glm::vec3(soundsVolume, 1.0f, 1.0f));
}
