#include "SkeletalModelImporter.h"

#include "Math/AssimpGLMHelper.h"
#include "Assets/AssetManager.h"
#include "Renderer/Bone.h"

#define SKELETAL_JOINTS_MAX 4

SkeletalModelImporter::SkeletalModelImporter()
{
}

Ref<SkeletalModel> SkeletalModelImporter::ImportSkeletalModel(std::string path)
{
	if (m_ImportedSkeletalModels.find(path) != m_ImportedSkeletalModels.end())
		return m_ImportedSkeletalModels.at(path);

	// Load model with Assimp
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Loading skeletal model failed: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}
	if (scene->HasAnimations() == false)
	{
		std::cout << "Provided model must have animations to be a Skeletal Model. Add Static Mesh instead" << std::endl;
		return nullptr;
	}
	glm::mat4 globalInverseTransform = AssimpGLMHelpers::ConvertMatrixToGLMFormat((scene->mRootNode->mTransformation).Inverse());


	// Specify Model requirements that need all meshes info to be constructed
	Ref<Rig> rig = CreateRef<Rig>();
	std::vector<Ref<SkeletalMesh>> meshes;

	// Process loaded meshes from all nodes
	ProcessNode(scene->mRootNode, scene, meshes, rig);

	std::string relativePath = path.substr(AssetManager::ContentDirectory.size() + 1);
	Ref<SkeletalModel> importedSkeletalModel = SkeletalModel::Create(relativePath, meshes, rig, globalInverseTransform);


	m_ImportedSkeletalModels.insert({ path, importedSkeletalModel });

	return importedSkeletalModel;
}

void SkeletalModelImporter::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Ref<SkeletalMesh>>& meshes, Ref<Rig> rig)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, rig));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, meshes, rig);
	}
}

Ref<SkeletalMesh> SkeletalModelImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene, Ref<Rig> rig)
{
	std::vector<SkinnedVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int totalBones = mesh->mNumBones;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		SkinnedVertex vertex;
		SetVertexBoneDataToDefault(vertex);

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

	if (mesh->HasBones())
	{
		ProcessMeshBones(vertices, mesh, rig);
	}

	return CreateRef<SkeletalMesh>(vertices, indices);
}

void SkeletalModelImporter::SetVertexBoneData(SkinnedVertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < SKELETAL_JOINTS_MAX; ++i)
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
	for (int i = 0; i < SKELETAL_JOINTS_MAX; i++)
	{
		vertex.BoneIDs[i] = -1;
		vertex.Weights[i] = 0.0f;
	}
}

void SkeletalModelImporter::ProcessMeshBones(std::vector<SkinnedVertex>& vertices, aiMesh* mesh, Ref<Rig> rig)
{
	// Loop through every bone
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
	{
		aiBone* assimpBone = mesh->mBones[boneIndex];
		std::string boneName = assimpBone->mName.C_Str();
		glm::mat4 offsetMatrix = AssimpGLMHelpers::ConvertMatrixToGLMFormat(assimpBone->mOffsetMatrix);


		// Assign mapping for bones
		int boneID = -1;
		Ref<BoneMap> bone = rig->FindBone(boneName);

		if (bone == nullptr)  // Not found: add new bone
		{
			rig->AddBone(boneName, offsetMatrix);
			boneID = rig->HowManyBones() - 1;
		}
		else  // Found bone: get id
		{
			boneID = bone->ID;
		}
		assert(boneID != -1);  // Debug-only


		// Assign weights and boneID to vertices
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		//std::cout << "\nnumWeights: " << numWeights;

		for (int weightIndex = 0; weightIndex < numWeights; weightIndex++)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}

}
