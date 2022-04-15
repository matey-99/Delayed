#include "SceneImporter.h"

#include "Scene/SceneSerializer.h"

SceneImporter::SceneImporter()
{
}

Ref<Scene> SceneImporter::ImportScene(std::string path)
{
	if (m_ImportedScenes.find(path) != m_ImportedScenes.end())
		return m_ImportedScenes.at(path);

	Ref<Scene> scene = SceneSerializer::Deserialize(path);
	if (!scene)
		return nullptr;

	m_ImportedScenes.insert({ path, scene });
	return scene;
}

void SceneImporter::ExportScene(Ref<Scene> scene, std::string destinationPath)
{
	SceneSerializer::Serialize(scene, destinationPath);
}
