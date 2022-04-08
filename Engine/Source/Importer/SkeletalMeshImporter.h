#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core.h"
#include "Renderer/SkeletalMesh.h"
#include "Math/AssimpGLMHelper.h"


class SkeletalMeshImporter
{
public:
	SkeletalMeshImporter();
	~SkeletalMeshImporter() {};

	SkeletalMeshImporter(SkeletalMeshImporter& other) = delete;
	void operator=(const SkeletalMeshImporter&) = delete;

	static Ref<SkeletalMeshImporter> GetInstance();

	std::vector<Ref<SkeletalMesh>> ImportMesh(std::string path);

	void SetVertexBoneData(SkinnedVertex& vertex, int boneID, float weight);
	void SetVertexBoneDataToDefault(SkinnedVertex& vertex);
	void ExtractBoneWeightForVertices(
		std::vector<SkinnedVertex>& vertices,
		aiMesh* mesh,
		const aiScene* scene,
		uint32_t &boneCounter);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Ref<SkeletalMesh>>& meshes);
	Ref<SkeletalMesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
	static Ref<SkeletalMeshImporter> s_Instance;
	static std::mutex s_Mutex;

	std::unordered_map<std::string, std::vector<Ref<SkeletalMesh>>> m_ImportedMeshes;
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;  // or map

};
