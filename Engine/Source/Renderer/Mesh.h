#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"
#include "Patterns/Container.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class Mesh
{
public:
	Mesh(std::vector<uint32_t> indices);
	void Render();
	void RenderInstanced(uint32_t instancesCount, std::vector<glm::mat4> modelMatrices);

	void CreateBounds(std::vector<Vertex> vertices);

	inline BoundingBox GetBoundingBox() const { return m_BoundingBox; }
    inline BoundingSphere GetBoundingSphere() const { return m_BoundingSphere; }
	inline std::vector<uint32_t> GetIndices() const { return m_Indices; }
	inline uint32_t GetVAO() const { return m_VAO; }

protected:
	virtual void SetupMesh() = 0;

protected:
	std::vector<uint32_t> m_Indices;

	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_EBO;

	uint32_t m_TransformationVBO;

private:
	BoundingBox m_BoundingBox;
    BoundingSphere m_BoundingSphere;
};