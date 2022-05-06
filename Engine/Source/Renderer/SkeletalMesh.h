#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"
#include <vector>
#include <unordered_map>
//#include "Bone.h"
#include "Mesh.h"

#define MAX_BONES_INFLUENCE 4


struct SkinnedVertex : Vertex
{
	int BoneIDs[MAX_BONES_INFLUENCE];  // global bone ids should be the same as in Rig class
	float Weights[MAX_BONES_INFLUENCE];  // weights for each bone in this specific vertex
};

class SkeletalMesh : public MeshBase
{
public:
	SkeletalMesh(std::vector<SkinnedVertex> vertices,
		std::vector<uint32_t> indices);

	inline std::vector<SkinnedVertex> GetVertices() const { return m_Vertices; }
	inline int const GetMaxBonesInfluence() const { return MAX_BONES_INFLUENCE; }

	//uint32_t GetBoneCount() { return m_BoneCounter; }
	//std::unordered_map<std::string, BoneInfo> GetBoneInfoMap() { return m_BoneInfoMap; }

	//void SetBoneMatrices(std::vector<glm::mat4> boneMatrices);
	/*std::vector<glm::mat4> GetBoneMatrices()
	{
		return m_BoneMatrices;
	}*/

private:
	void SetupMesh();

public:
	std::vector<SkinnedVertex> m_Vertices;
	//uint32_t m_BoneCounter = 0;
	//std::vector<glm::mat4> m_BoneMatrices;
	//std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;  // or map
};