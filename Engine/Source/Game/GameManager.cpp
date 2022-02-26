#include "GameManager.h"

Ref<GameManager> GameManager::s_Instance{};
std::mutex GameManager::s_Mutex;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

Ref<GameManager> GameManager::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (!s_Instance)
		s_Instance = CreateRef<GameManager>();

	return s_Instance;
}