#include "SkeletalModelImporter.h"

#include "Math/AssimpGLMHelper.h"
#include "Scene/Component/Animation/Animation.h"
#include "Assets/AssetManager.h"

SkeletalModelImporter::SkeletalModelImporter()
{
}

Ref<SkeletalModel> SkeletalModelImporter::ImportSkeletalModel(std::string path)
{
	if (m_ImportedSkeletalModels.find(path) != m_ImportedSkeletalModels.end())
		return m_ImportedSkeletalModels.at(path);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Loading skeletal model failed: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	std::vector<Ref<SkeletalMesh>> meshes;
	ProcessNode(scene->mRootNode, scene, meshes);

	std::string relativePath = path.substr(AssetManager::ContentDirectory.size() + 1);
	Ref<SkeletalModel> importedSkeletalModel = SkeletalModel::Create(relativePath, meshes);

	// Load animations and save them to SkeletalModel
	for (int index = 0; index < scene->mNumAnimations; index++)
	{
		auto _animation = CreateRef<Animation>(scene, index, meshes[0]);

		std::cout << "Added animation to Skeletal Model, its name: " << _animation->GetAnimationName() << "\n";

		importedSkeletalModel->AddAnimation(_animation);
	}
	/*for (auto& mesh : meshes)
		importedSkeletalModel->LoadAnimation(CreateRef<Animation>(scene, mesh));*/

	m_ImportedSkeletalModels.insert({ path, importedSkeletalModel });

	//m_AnimationCounter = scene->mNumAnimations;
	// Import animations

	return importedSkeletalModel;
}

void SkeletalModelImporter::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Ref<SkeletalMesh>>& meshes)
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

Ref<SkeletalMesh> SkeletalModelImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<SkinnedVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		SkinnedVertex vertex;
		SetVertexBoneDataToDefault(vertex);
		/*vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
		vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);*/

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;

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

	uint32_t boneCounter = 0;
	std::unordered_map<std::string, BoneInfo> boneInfoMap;

	if (mesh->HasBones())
	{
		ExtractBoneWeightForVertices(vertices, mesh, scene, boneCounter);
	}

	return CreateRef<SkeletalMesh>(vertices, indices, boneCounter, m_BoneInfoMap);
}

void SkeletalModelImporter::SetVertexBoneData(SkinnedVertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < 4; ++i)
	{
		if (vertex.BoneIDs[i] < 0)
		{
			vertex.Weights[i] = weight;
			vertex.BoneIDs[i] = boneID;

			break;
		}
	}
}

void SkeletalModelImporter::SetVertexBoneDataToDefault(SkinnedVertex& vertex)
{
	const int MAX_BONE_INFLUENCE = 4;

	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.BoneIDs[i] = -1;
		vertex.Weights[i] = 0.0f;
	}
}

void SkeletalModelImporter::ExtractBoneWeightForVertices(
	std::vector<SkinnedVertex>& vertices,
	aiMesh* mesh,
	const aiScene* scene,
	uint32_t& boneCounter)
{
	auto& boneInfoMap = m_BoneInfoMap;

	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.ID = boneCounter;
			newBoneInfo.Offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCounter;
			boneCounter++;
		}
		else
		{
			boneID = boneInfoMap[boneName].ID;
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
