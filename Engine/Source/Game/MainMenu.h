#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Patterns/Singleton.h"

class ButtonComponent;

class MainMenu : public GameComponent, public Singleton<MainMenu>
{
public:
	MainMenu(Actor* owner);
	~MainMenu();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	void Play();
	void OpenOptions();
	void CloseOptions();
	void Exit();

private:
	Ref<ButtonComponent> m_PlayButton;
	Ref<ButtonComponent> m_OptionsButton;
	Ref<ButtonComponent> m_ExitButton;

	Ref<Actor> m_Options;

#pragma region Serialization

	uint64_t m_PlayButtonID;
	uint64_t m_OptionsButtonID;
	uint64_t m_ExitButtonID;

#pragma endregion

	friend class SceneSerializer;
};