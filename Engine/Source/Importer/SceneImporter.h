#pragma once

#include "Core.h"

class Scene;

class SceneImporter
{
public:
	SceneImporter();

	Ref<Scene> ImportScene(std::string path);
	void ExportScene(Ref<Scene> scene, std::string destinationPath);

private:
	std::unordered_map<std::string, Ref<Scene>> m_ImportedScenes;
};