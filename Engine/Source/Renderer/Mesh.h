#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> inVertices, std::vector<uint32_t> inIndices, bool instanced = false);
	void Render();
	void RenderInstanced(uint32_t count);

	inline BoundingBox GetBoundingBox() const { return m_BoundingBox; }
    inline BoundingSphere GetBoundingSphere() const { return m_BoundingSphere; }
	inline std::vector<Vertex> GetVertices() const { return m_Vertices; }
	inline std::vector<uint32_t> GetIndices() const { return m_Indices; }
	inline uint32_t GetVAO() const { return m_VAO; }

private:
	void SetupMesh();
	void SetupMeshInstanced();

private:
	BoundingBox m_BoundingBox;
    BoundingSphere m_BoundingSphere;

	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_EBO;
};