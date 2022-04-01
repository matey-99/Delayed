#include "SkeletalMeshImporter.h"

#include "Content/ContentHelper.h"
#include "Scene/Component/SkeletalMeshComponent.h"  // ? because AssimpGLMHelpes is there
#include <iostream>

Ref<SkeletalMeshImporter> SkeletalMeshImporter::s_Instance{};
std::mutex SkeletalMeshImporter::s_Mutex;

SkeletalMeshImporter::SkeletalMeshImporter()
{
	m_ImportedMeshes = std::unordered_map<std::string, std::vector<SkeletalMesh>>();
}

Ref<SkeletalMeshImporter> SkeletalMeshImporter::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<SkeletalMeshImporter>();

	return s_Instance;
}

std::vector<SkeletalMesh> SkeletalMeshImporter::ImportMesh(std::string path)
{
	if (m_ImportedMeshes.find(path) != m_ImportedMeshes.end())
		return m_ImportedMeshes.at(path);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(ContentHelper::GetAssetPath(path), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Loading model failed: " << importer.GetErrorString() << std::endl;
		return std::vector<SkeletalMesh>();
	}

	std::vector<SkeletalMesh> meshes = std::vector<SkeletalMesh>();

	ProcessNode(scene->mRootNode, scene, meshes);

	m_ImportedMeshes.insert({ path, meshes });
	return meshes;
}

void SkeletalMeshImporter::SetVertexBoneData(SkinnedVertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < 4; ++i)
	{
		if (vertex.boneIDs[i] < 0)
		{
			vertex.weights[i] = weight;
			vertex.boneIDs[i] = boneID;
			break;
		}
	}
}

void SkeletalMeshImporter::SetVertexBoneDataToDefault(SkinnedVertex& vertex)
{
	const int MAX_BONE_INFLUENCE = 4;

	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.boneIDs[i] = -1;
		vertex.weights[i] = 0.0f;
	}
}

void SkeletalMeshImporter::ExtractBoneWeightForVertices(std::vector<SkinnedVertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	auto& boneInfoMap = m_BoneInfoMap;
	int& boneCount = m_BoneCounter;

	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

void SkeletalMeshImporter::ProcessNode(aiNode* node, const aiScene* scene, std::vector<SkeletalMesh>& meshes)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, meshes);
	}
}

SkeletalMesh SkeletalMeshImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<SkinnedVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		SkinnedVertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return SkeletalMesh(vertices, indices);
}
