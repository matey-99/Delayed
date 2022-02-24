#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "typedefs.h"
#include "Renderer/Mesh.h"

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter() {};

	MeshImporter(MeshImporter& other) = delete;
	void operator=(const MeshImporter&) = delete;

	static Ref<MeshImporter> GetInstance();

	std::vector<Mesh> ImportMesh(std::string path);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
	static Ref<MeshImporter> s_Instance;
	static std::mutex s_Mutex;

	std::unordered_map<std::string, std::vector<Mesh>> m_ImportedMeshes;
};