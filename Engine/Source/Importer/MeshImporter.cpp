#include "MeshImporter.h"

#include "Content/ContentHelper.h"

Ref<MeshImporter> MeshImporter::s_Instance{};
std::mutex MeshImporter::s_Mutex;

MeshImporter::MeshImporter()
{
	m_ImportedMeshes = std::unordered_map<std::string, std::vector<Ref<StaticMesh>>>();
}

Ref<MeshImporter> MeshImporter::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<MeshImporter>();

	return s_Instance;
}

std::vector<Ref<StaticMesh>> MeshImporter::ImportMesh(std::string path)
{
	if (m_ImportedMeshes.find(path) != m_ImportedMeshes.end())
		return m_ImportedMeshes.at(path);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(ContentHelper::GetAssetPath(path), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Loading model failed: " << importer.GetErrorString() << std::endl;
		return std::vector<Ref<StaticMesh>>();
	}

	std::vector<Ref<StaticMesh>> meshes = std::vector<Ref<StaticMesh>>();

	ProcessNode(scene->mRootNode, scene, meshes);

	m_ImportedMeshes.insert({ path, meshes });
	return meshes;
}

void MeshImporter::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Ref<StaticMesh>>& meshes)
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

Ref<StaticMesh> MeshImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

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

	return CreateRef<StaticMesh>(vertices, indices);
}
