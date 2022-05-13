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

	inline static SaveManager* GetInstance() { return s_Instance; }

private:
	void GetAllSaveables();

private:
	static SaveManager* s_Instance;
	std::vector<Ref<Saveable>> m_Saveables;
};