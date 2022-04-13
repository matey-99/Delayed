#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"
#include <vector>
#include <unordered_map>

#include "Mesh.h"

#define MAX_BONES_INFLUENCE 4


struct BoneInfo
{
	int ID;				// index in finalBoneMatrices
	glm::mat4 Offset;	// offset matrix transforms vertex from model space to bone space
};

struct SkinnedVertex : Vertex
{
	int BoneIDs[MAX_BONES_INFLUENCE];
	float Weights[MAX_BONES_INFLUENCE];
};

class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh(std::vector<SkinnedVertex> vertices,
		std::vector<uint32_t> indices,
		uint32_t boneCounter);

	inline std::vector<SkinnedVertex> GetVertices() const { return m_Vertices; }
	inline int const GetMaxBonesInfluence() const { return MAX_BONES_INFLUENCE; }

	uint32_t GetBoneCount() { return m_BoneCounter; }

private:
	void SetupMesh();

private:
	std::vector<SkinnedVertex> m_Vertices;
	uint32_t m_BoneCounter = 0;
	//std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;  // or map
};