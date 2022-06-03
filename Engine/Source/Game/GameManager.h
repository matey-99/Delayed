#pragma once

#include "Core.h"
#include "GameComponent.h"

class GameManager : public GameComponent
{
public:
	GameManager(Actor* owner);

	virtual void Start() override;

	void PauseGame();
	void ResumeGame();

	inline static GameManager* GetInstance() { return s_Instance; }
	inline bool IsGamePaused() const { return m_IsGamePaused; }

private:
	void HandleGameState();
	void EnableHandleGameState();

private:
	static GameManager* s_Instance;
	bool m_IsGamePaused;

	Ref<Actor> m_MainMenu;

	bool m_CanHandleGameState;

#pragma region Serialization

	uint64_t m_MainMenuID;

#pragma endregion

	friend class SceneSerializer;
};