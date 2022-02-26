#include "SceneManager.h"
#include "SceneSerializer.h"

Ref<SceneManager> SceneManager::s_Instance{};
std::mutex SceneManager::s_Mutex;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

Ref<SceneManager> SceneManager::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (!s_Instance)
		s_Instance = CreateRef<SceneManager>();

	return s_Instance;
}

void SceneManager::CreateScene(std::string name)
{
	m_CurrentScene = CreateRef<Scene>();
	m_CurrentScene->SetName(name);

	m_CurrentScenePath = "../../../Assets/Scenes/" + name + ".scene";

	SceneSerializer::Serialize(m_CurrentScene, m_CurrentScenePath);
}

void SceneManager::LoadScene(std::string path)
{
	m_CurrentScene = SceneSerializer::Deserialize(path);
	m_CurrentScenePath = path;
}
