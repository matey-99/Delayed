#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "typedefs.h"
#include "Renderer/SkeletalMesh.h"

class AssimpGLMHelpers;

struct BoneInfo
{
	int id;				// index in finalBoneMatrices
	glm::mat4 offset;	// offset matrix transforms vertex from model space to bone space
};

class SkeletalMeshImporter
{
public:
	SkeletalMeshImporter();
	~SkeletalMeshImporter() {};

	SkeletalMeshImporter(SkeletalMeshImporter& other) = delete;
	void operator=(const SkeletalMeshImporter&) = delete;

	static Ref<SkeletalMeshImporter> GetInstance();

	std::vector<SkeletalMesh> ImportMesh(std::string path);

	void SetVertexBoneData(SkinnedVertex& vertex, int boneID, float weight);
	void SetVertexBoneDataToDefault(SkinnedVertex& vertex);
	void ExtractBoneWeightForVertices(std::vector<SkinnedVertex>& vertices, aiMesh* mesh, const aiScene* scene);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<SkeletalMesh>& meshes);
	SkeletalMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
	static Ref<SkeletalMeshImporter> s_Instance;
	static std::mutex s_Mutex;

	std::unordered_map<std::string, std::vector<SkeletalMesh>> m_ImportedMeshes;
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;  // or map
	int m_BoneCounter = 0;
};
