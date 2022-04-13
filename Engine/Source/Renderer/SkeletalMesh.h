#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"
#include <vector>
#include <unordered_map>

<<<<<<< Updated upstream
#define MAX_BONES_INFLUENCE 4


struct BoneInfo
{
	int id;				// index in finalBoneMatrices
	glm::mat4 offset;	// offset matrix transforms vertex from model space to bone space
};

struct SkinnedVertex
=======
#include "../Renderer/Bone.h"
#include "Mesh.h"

#define MAX_BONES_INFLUENCE 4


struct SkinnedVertex : Vertex
>>>>>>> Stashed changes
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	int boneIDs[MAX_BONES_INFLUENCE];
	float weights[MAX_BONES_INFLUENCE];
};

class SkeletalMesh
{
public:
<<<<<<< Updated upstream
	SkeletalMesh(std::vector<SkinnedVertex> inVertices,
		std::vector<uint32_t> inIndices,
		uint32_t boneCounter);
	void Render();
=======
	SkeletalMesh(std::vector<SkinnedVertex> vertices,
		std::vector<uint32_t> indices,
		uint32_t boneCounter,
		std::unordered_map<std::string, BoneInfo> boneInfoMap);
>>>>>>> Stashed changes

	inline BoundingBox GetBoundingBox() const { return m_BoundingBox; }
	inline BoundingSphere GetBoundingSphere() const { return m_BoundingSphere; }
	inline std::vector<SkinnedVertex> GetVertices() const { return m_Vertices; }
	inline std::vector<uint32_t> GetIndices() const { return m_Indices; }
	inline uint32_t GetVAO() const { return m_VAO; }
	inline int const GetMaxBonesInfluence() const { return MAX_BONES_INFLUENCE; }

	uint32_t GetBoneCount() { return m_BoneCounter; }
	std::unordered_map<std::string, BoneInfo> GetBoneInfoMap() { return m_BoneInfoMap; }

private:
	void SetupMesh();

private:
	BoundingBox m_BoundingBox;
	BoundingSphere m_BoundingSphere;

	std::vector<SkinnedVertex> m_Vertices;
<<<<<<< Updated upstream
	std::vector<uint32_t> m_Indices;

	uint32_t m_BoneCounter = 0;
	//std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;  // or map

	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_EBO;
=======
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;  // or map
public:
	uint32_t m_BoneCounter = 0;
>>>>>>> Stashed changes
};