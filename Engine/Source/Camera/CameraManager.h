#pragma once

#include <mutex>

#include "Camera.h"
#include "typedefs.h"

class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	CameraManager(CameraManager& other) = delete;
	void operator=(const CameraManager&) = delete;

	static Ref<CameraManager> GetInstance();

	inline Ref<Camera> GetMainCamera() const { return m_MainCamera; }
	inline void SetMainCamera(Ref<Camera> camera) { m_MainCamera = camera; }

private:
	static Ref<CameraManager> s_Instance;
	static std::mutex s_Mutex;

	Ref<Camera> m_MainCamera;
};