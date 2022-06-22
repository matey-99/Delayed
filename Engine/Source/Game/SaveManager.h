#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

class Saveable;

class SaveManager : public Singleton<SaveManager>
{
public:
	void Initialize();
	void Start();

	void SaveGame();
	bool LoadGame();

    static bool IsSaveAvailable();

	inline bool IsLoadGameOnStart() const { return m_LoadGameOnStart; }
	inline void SetLoadGameOnStart(bool loadGame) { m_LoadGameOnStart = loadGame; }

	void GetAllSaveables();

private:
	std::vector<Ref<Saveable>> m_Saveables;
	bool m_LoadGameOnStart;
};