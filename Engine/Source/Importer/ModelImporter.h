#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Assets/Model.h"

class ModelImporter : public Singleton<ModelImporter>
{
public:
	ModelImporter();

	Ref<Model> ImportModel(std::string path);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Ref<Mesh>>& meshes);
	Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
	std::unordered_map<std::string, Ref<Model>> m_ImportedModels;
};