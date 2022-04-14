#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Assets/SkeletalModel.h"
#include "Renderer/SkeletalMesh.h"

class SkeletalModelImporter : public Singleton<SkeletalModelImporter>
{
public:
	Ref<SkeletalModel> ImportSkeletalModel(std::string path);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Ref<SkeletalMesh>>& meshes);
	Ref<SkeletalMesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

	void SetVertexBoneData(SkinnedVertex& vertex, int boneID, float weight);
	void SetVertexBoneDataToDefault(SkinnedVertex& vertex);
	void ExtractBoneWeightForVertices(
		std::vector<SkinnedVertex>& vertices,
		aiMesh* mesh,
		const aiScene* scene,
		uint32_t& boneCounter);

private:
	std::unordered_map<std::string, Ref<SkeletalModel>> m_ImportedSkeletalModels;
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;  // or map
};
