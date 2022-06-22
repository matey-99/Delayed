#include "SceneManager.h"
#include "SceneSerializer.h"

#include "Assets/AssetManager.h"
#include "Input/Input.h"
#include "Game/SaveManager.h"

Ref<Scene> SceneManager::CreateScene(std::string name)
{
	m_CurrentScene = CreateRef<Scene>();
	m_CurrentScene->SetName(name);

	m_CurrentScenePath = "../../../Content/Scenes/" + name + ".scene";

	SceneSerializer::Serialize(m_CurrentScene, m_CurrentScenePath);

	return m_CurrentScene;
}

Ref<Scene> SceneManager::LoadScene(std::string path)
{
	if (m_CurrentScene)
		m_CurrentScene->Destroy();

	Input::GetInstance()->Reset();

	m_CurrentScene = AssetManager::LoadScene(path);
	m_CurrentScenePath = path;

	m_CurrentScene->Start();
	SaveManager::GetInstance()->Start();

	return m_CurrentScene;
}
