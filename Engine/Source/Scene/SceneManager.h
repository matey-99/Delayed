#pragma once

#include "typedefs.h"
#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	SceneManager(SceneManager& other) = delete;
	void operator=(const SceneManager&) = delete;

	static Ref<SceneManager> GetInstance();

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