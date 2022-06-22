#pragma once

#include "Core.h"
#include "GameComponent.h"

class Saveable;

class SaveManager : public GameComponent
{
public:
	SaveManager(Actor* owner);

	virtual void Start() override;

	void SaveGame();
	bool LoadGame();

    static bool IsSaveAvailable();

	inline static SaveManager* GetInstance() { return s_Instance; }

	inline bool IsLoadGameOnStart() const { return m_LoadGameOnStart; }
	inline void SetLoadGameOnStart(bool loadGame) { m_LoadGameOnStart = loadGame; }

private:
	void GetAllSaveables();

private:
	static SaveManager* s_Instance;
	std::vector<Ref<Saveable>> m_Saveables;
	bool m_LoadGameOnStart;
};