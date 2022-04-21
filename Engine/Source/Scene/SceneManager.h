#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

#include "Scene.h"

class SceneManager : public Singleton<SceneManager>
{
public:
	Ref<Scene> CreateScene(std::string name);
	Ref<Scene> LoadScene(std::string path);

	inline Ref<Scene> GetCurrentScene() const { return m_CurrentScene; }
	inline std::string GetCurrentScenePath() const { return m_CurrentScenePath; }

private:
	static Ref<SceneManager> s_Instance;
	static std::mutex s_Mutex;

	Ref<Scene> m_CurrentScene;
	std::string m_CurrentScenePath;
};