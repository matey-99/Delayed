#pragma once

#include <mutex>

#include "typedefs.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	GameManager(GameManager& other) = delete;
	void operator=(const GameManager&) = delete;

	static Ref<GameManager> GetInstance();

private:
	static Ref<GameManager> s_Instance;
	static std::mutex s_Mutex;
};