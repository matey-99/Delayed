#pragma once

#include "Core.h"
#include "GameComponent.h"

enum class TutorialType
{
	DoubleJump, Dash, Teleport
};

class TutorialManager : public GameComponent
{
public:
	TutorialManager(Actor* owner);

	virtual void Start() override;

	void DisplayTutorial(TutorialType type);
	void HideTutorial(TutorialType type);

	bool IsTutorialDisplayed(TutorialType type) const;

	inline bool IsTutorialEnabled() const { return m_TutorialEnabled; }

	inline static TutorialManager* GetInstance() { return s_Instance; }

private:
	static TutorialManager* s_Instance;

	bool m_TutorialEnabled;

	Ref<Actor> m_DoubleJumpTutorial;
	Ref<Actor> m_DashTutorial;
	Ref<Actor> m_TeleportTutorial;

#pragma region Serialization

	uint64_t m_DoubleJumpTutorialID;
	uint64_t m_DashTutorialID;
	uint64_t m_TeleportTutorialID;

#pragma endregion

	friend class SceneSerializer;
};