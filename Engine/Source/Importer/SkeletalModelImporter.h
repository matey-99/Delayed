#pragma once

#include "Core.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Assets/SkeletalModel.h"
#include "Renderer/SkeletalMesh.h"


class SkeletalModelImporter
{
public:
	SkeletalModelImporter();

	Ref<SkeletalModel> ImportSkeletalModel(std::string path);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Ref<SkeletalMesh>>& meshes, Ref<Rig> rig);
	Ref<SkeletalMesh> ProcessMesh(aiMesh* mesh, const aiScene* scene, Ref<Rig>);

	void SetVertexBoneData(SkinnedVertex& vertex, int boneID, float weight);
	void SetVertexBoneDataToDefault(SkinnedVertex& vertex);
	void ProcessMeshBones(std::vector<SkinnedVertex>& vertices, aiMesh* mesh, Ref<Rig> rig);

private:
	std::unordered_map<std::string, Ref<SkeletalModel>> m_ImportedSkeletalModels;
};
