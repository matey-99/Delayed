#include "SkeletalMesh.h"

#include <glad/glad.h>

<<<<<<< Updated upstream
SkeletalMesh::SkeletalMesh(std::vector<SkinnedVertex> inVertices,
	std::vector<uint32_t> inIndices,
	uint32_t boneCounter)
	:
	m_Vertices(inVertices), m_Indices(inIndices), m_BoneCounter(boneCounter)
=======
SkeletalMesh::SkeletalMesh(std::vector<SkinnedVertex> vertices,
	std::vector<uint32_t> indices,
	uint32_t boneCounter,
	std::unordered_map<std::string, BoneInfo> boneInfoMap)
	: m_Vertices(vertices), Mesh(indices), m_BoneCounter(boneCounter), m_BoneInfoMap(boneInfoMap)
>>>>>>> Stashed changes
{
	SetupMesh();

	std::vector<glm::vec3> points;
	for (auto vertex : m_Vertices)
		points.push_back(vertex.position);

	m_BoundingBox = BoundingBox(points);
	m_BoundingSphere = BoundingSphere(points);
}

void SkeletalMesh::Render()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SkeletalMesh::SetupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(SkinnedVertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)0);

	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, normal));

	// Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, texCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, tangent));

	// bone ids
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 4, GL_INT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, boneIDs));

	// weights
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, weights));



	glBindVertexArray(0);
}