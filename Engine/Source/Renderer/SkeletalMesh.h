#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Math/BoundingBox.h"
#include <vector>

#define MAX_BONES_INFLUENCE 4

struct SkinnedVertex
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
	SkeletalMesh(std::vector<SkinnedVertex> inVertices, std::vector<uint32_t> inIndices, bool instanced = false);
	void Render();

	inline BoundingBox GetBoundingBox() const { return m_BoundingBox; }
	inline std::vector<SkinnedVertex> GetVertices() const { return m_Vertices; }
	inline std::vector<uint32_t> GetIndices() const { return m_Indices; }
	inline uint32_t GetVAO() const { return m_VAO; }
	inline int const GetMaxBonesInfluence() const { return MAX_BONES_INFLUENCE; }

private:
	void SetupMesh();

private:
	BoundingBox m_BoundingBox;

	std::vector<SkinnedVertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_EBO;
};