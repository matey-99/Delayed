#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"
#include <vector>
#include <unordered_map>
#include "Bone.h"
#include "Mesh.h"

#define MAX_BONES_INFLUENCE 4


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
	std::unordered_map<std::string, BoneInfo> GetBoneInfoMap() { return m_BoneInfoMap; }

private:
	void SetupMesh();

public:
	std::vector<SkinnedVertex> m_Vertices;
	uint32_t m_BoneCounter = 0;
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;  // or map
};