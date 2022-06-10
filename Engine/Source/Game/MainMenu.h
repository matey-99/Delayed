#pragma once

#include "Core.h"

#include "GameComponent.h"

class ButtonComponent;
class TextComponent;

class MainMenu : public GameComponent
{
public:
	MainMenu(Actor* owner);
	~MainMenu();

	virtual void Start() override;

	/* Default Panel */
	void Play();
	void OpenOptions();
	void OpenCredits();
	void Exit();

	/* Options Panel */
	void IncreaseMasterVolume();
	void DecreaseMasterVolume();
	void IncreaseMusicVolume();
	void DecreaseMusicVolume();
	void IncreaseSoundsVolume();
	void DecreaseSoundsVolume();
	void CloseOptions();

	/* Credits Panel */
	void CloseCredits();

private:
	void UpdateOptions();

private:
	/* Default Panel */
	Ref<Actor> m_DefaultPanel;

	Ref<ButtonComponent> m_PlayButton;
	Ref<ButtonComponent> m_OptionsButton;
	Ref<ButtonComponent> m_CreditsButton;
	Ref<ButtonComponent> m_ExitButton;

	/* Options Panel */
	Ref<Actor> m_OptionsPanel;

	Ref<TextComponent> m_MasterVolumeText;
	Ref<Actor> m_MasterVolumeSlider;
	Ref<ButtonComponent> m_IncreaseMasterVolumeButton;
	Ref<ButtonComponent> m_DecreaseMasterVolumeButton;

	Ref<TextComponent> m_MusicVolumeText;
	Ref<Actor> m_MusicVolumeSlider;
	Ref<ButtonComponent> m_IncreaseMusicVolumeButton;
	Ref<ButtonComponent> m_DecreaseMusicVolumeButton;

	Ref<TextComponent> m_SoundsVolumeText;
	Ref<Actor> m_SoundsVolumeSlider;
	Ref<ButtonComponent> m_IncreaseSoundsVolumeButton;
	Ref<ButtonComponent> m_DecreaseSoundsVolumeButton;

	Ref<ButtonComponent> m_BackFromOptionsButton;

	/* Credits Panel */
	Ref<Actor> m_CreditsPanel;
	Ref<ButtonComponent> m_BackFromCreditsButton;


#pragma region Serialization

	/* Default Panel */
	uint64_t m_DefaultPanelID;

	uint64_t m_PlayButtonID;
	uint64_t m_OptionsButtonID;
	uint64_t m_CreditsButtonID;
	uint64_t m_ExitButtonID;

	/* Options Panel */
	uint64_t m_OptionsPanelID;

	uint64_t m_MasterVolumeTextID;
	uint64_t m_IncreaseMasterVolumeButtonID;
	uint64_t m_DecreaseMasterVolumeButtonID;
	uint64_t m_MasterVolumeSliderID;

	uint64_t m_MusicVolumeTextID;
	uint64_t m_IncreaseMusicVolumeButtonID;
	uint64_t m_DecreaseMusicVolumeButtonID;
	uint64_t m_MusicVolumeSliderID;

	uint64_t m_SoundsVolumeTextID;
	uint64_t m_IncreaseSoundsVolumeButtonID;
	uint64_t m_DecreaseSoundsVolumeButtonID;
	uint64_t m_SoundsVolumeSliderID;

	uint64_t m_BackFromOptionsButtonID;

	/* Credits Panel */
	uint64_t m_CreditsPanelID;
	uint64_t m_BackFromCreditsButtonID;


#pragma endregion

	friend class SceneSerializer;
};