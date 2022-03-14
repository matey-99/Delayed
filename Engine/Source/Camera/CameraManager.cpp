#include "CameraManager.h"

Ref<CameraManager> CameraManager::s_Instance{};
std::mutex CameraManager::s_Mutex;

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

Ref<CameraManager> CameraManager::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (!s_Instance)
		s_Instance = CreateRef<CameraManager>();

	return s_Instance;
}